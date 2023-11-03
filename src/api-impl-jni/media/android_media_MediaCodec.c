/*
 * Android MediaCodec implementation for Android Translation Layer
 *
 * Large part of the video decoding code was taken from https://git.sr.ht/~emersion/vaapi-decoder/tree/wayland/
 */

#include <libavutil/frame.h>
#include <stdio.h>
#include <EGL/eglplatform.h>

#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <gdk/wayland/gdkwayland.h>

#include <libavcodec/avcodec.h>
#include <libavutil/hwcontext_drm.h>
#include <libavutil/pixdesc.h>
#include <drm_fourcc.h>
#include <libswresample/swresample.h>

#include <stdlib.h>
#include <wayland-client.h>
#include "linux-dmabuf-unstable-v1-client-protocol.h"
#include "viewporter-client-protocol.h"

#include "jni.h"
#include "../generated_headers/android_media_MediaCodec.h"
#include "../util.h"
#include "src/api-impl-jni/defines.h"
#include "../../libandroid/native_window.h"

struct ATL_codec_context {
	AVCodecContext *codec;
	union {
		struct {
			SwrContext *swr;
		} audio;
		struct {
			struct zwp_linux_dmabuf_v1 *zwp_linux_dmabuf_v1;
			struct wp_viewporter *wp_viewporter;
			struct wp_viewport *viewport;
			struct ANativeWindow *native_window;
			int surface_width;
			int surface_height;
		} video;
	};
};

static enum AVPixelFormat hw_pix_fmt = AV_PIX_FMT_NONE;

JNIEXPORT jlong JNICALL Java_android_media_MediaCodec_native_1constructor(JNIEnv *env, jobject this, jstring codec_name)
{
	const char *name = (*env)->GetStringUTFChars(env, codec_name, NULL);
	const AVCodec *codec = avcodec_find_decoder_by_name(name);
	(*env)->ReleaseStringUTFChars(env, codec_name, name);
	AVCodecContext *codec_ctx = avcodec_alloc_context3(codec);

	struct ATL_codec_context *ctx = calloc(1, sizeof(struct ATL_codec_context));
	ctx->codec = codec_ctx;
	return _INTPTR(ctx);
}

JNIEXPORT void JNICALL Java_android_media_MediaCodec_native_1configure_1audio(JNIEnv *env, jobject this, jlong codec, jobject extradata, jint sample_rate, jint nb_channels)
{
	struct ATL_codec_context *ctx = _PTR(codec);
	AVCodecContext *codec_ctx = ctx->codec;
	jarray array_ref;
	jbyte *array;
	void *data;

	printf("Java_android_media_MediaCodec_native_1configure_1audio(%s, %d, %d)\n", codec_ctx->codec->name, sample_rate, nb_channels);

	codec_ctx->sample_rate = sample_rate;
	codec_ctx->ch_layout = (AVChannelLayout) AV_CHANNEL_LAYOUT_STEREO;
	codec_ctx->ch_layout.nb_channels = nb_channels;

	data = get_nio_buffer(env, extradata, &array_ref, &array);
	codec_ctx->extradata_size = get_nio_buffer_size(env, extradata);
	codec_ctx->extradata = av_mallocz(codec_ctx->extradata_size + AV_INPUT_BUFFER_PADDING_SIZE);
	memcpy(codec_ctx->extradata, data, codec_ctx->extradata_size);
	release_nio_buffer(env, array_ref, array);

	for (int i = 0; i < codec_ctx->extradata_size; i++) {
		printf("params->extradata[%d] = %x\n", i, codec_ctx->extradata[i]);
	}
}

/*
 * Helper functions for hardware accelerated video decoding using Wayland DMA-Buf protocol
 */

static const struct {
	uint32_t format;
	int nb_layers;
	uint32_t layers[AV_DRM_MAX_PLANES];
} drm_format_map[] = {
	{ DRM_FORMAT_NV12, 2, { DRM_FORMAT_R8, DRM_FORMAT_GR88 } },
};

static uint32_t get_drm_frame_format(const AVDRMFrameDescriptor *drm_frame_desc)
{
	if (drm_frame_desc->nb_layers == 1) {
		return drm_frame_desc->layers[0].format;
	}
	for (size_t i = 0; i < sizeof(drm_format_map) / sizeof(drm_format_map[0]); i++) {
		if (drm_format_map[i].nb_layers != drm_frame_desc->nb_layers) {
			continue;
		}
		int match = 1;
		for (int j = 0; j < drm_frame_desc->nb_layers; j++) {
			match &= drm_frame_desc->layers[j].format == drm_format_map[i].layers[j];
		}
		if (match) {
			return drm_format_map[i].format;
		}
	}
	return DRM_FORMAT_INVALID;
}

static int check_hw_device_type(enum AVHWDeviceType type)
{
	enum AVHWDeviceType t = AV_HWDEVICE_TYPE_NONE;
	while (1) {
		t = av_hwdevice_iterate_types(t);
		if (t == AV_HWDEVICE_TYPE_NONE) {
			break;
		}
		if (t == type) {
			return 0;
		}
	}
	return -1;
}

static enum AVPixelFormat get_hw_format(AVCodecContext *ctx,
		const enum AVPixelFormat *pix_fmts)
{
	for (size_t i = 0; pix_fmts[i] != AV_PIX_FMT_NONE; i++) {
		if (pix_fmts[i] == hw_pix_fmt) {
			return hw_pix_fmt;
		}
	}

	fprintf(stderr, "Failed to find HW pixel format\n");
	return AV_PIX_FMT_NONE;
}

static void handle_global(void *data, struct wl_registry *registry,
		uint32_t name, const char *interface, uint32_t version)
{
	struct ATL_codec_context *ctx = data;
	if (strcmp(interface, zwp_linux_dmabuf_v1_interface.name) == 0) {
		ctx->video.zwp_linux_dmabuf_v1 =
			wl_registry_bind(registry, name, &zwp_linux_dmabuf_v1_interface, 2);
	} else if (strcmp(interface, wp_viewporter_interface.name) == 0) {
		ctx->video.wp_viewporter =
			wl_registry_bind(registry, name, &wp_viewporter_interface, 1);
	}
}

static void handle_global_remove(void *data, struct wl_registry *registry,
		uint32_t name)
{
	// This space is intentionally left blank
}

static const struct wl_registry_listener registry_listener = {
	.global = handle_global,
	.global_remove = handle_global_remove,
};

static struct wl_buffer *import_drm_frame_desc(struct zwp_linux_dmabuf_v1 *zwp_linux_dmabuf_v1,
		const AVDRMFrameDescriptor *drm_frame_desc, int width, int height)
{
	// VA-API drivers may use separate layers with one plane each, or a single
	// layer with multiple planes. We need to handle both.
	uint32_t drm_format = get_drm_frame_format(drm_frame_desc);
	if (drm_format == DRM_FORMAT_INVALID) {
		fprintf(stderr, "Failed to get DRM frame format\n");
		return NULL;
	}
	// fprintf(stderr, "DRM format: 0x%X\n", drm_format);

	struct zwp_linux_buffer_params_v1 *dmabuf_params =
		zwp_linux_dmabuf_v1_create_params(zwp_linux_dmabuf_v1);
	int k = 0;
	for (int i = 0; i < drm_frame_desc->nb_layers; i++) {
		const AVDRMLayerDescriptor *drm_layer = &drm_frame_desc->layers[i];

		for (int j = 0; j < drm_layer->nb_planes; j++) {
			const AVDRMPlaneDescriptor *drm_plane = &drm_layer->planes[j];
			const AVDRMObjectDescriptor *drm_object =
				&drm_frame_desc->objects[drm_plane->object_index];

			uint32_t modifier_hi = drm_object->format_modifier >> 32;
			uint32_t modifier_lo = drm_object->format_modifier & 0xFFFFFFFF;

			zwp_linux_buffer_params_v1_add(dmabuf_params, drm_object->fd, k,
				drm_plane->offset, drm_plane->pitch, modifier_hi, modifier_lo);
			k++;
		}
	}

	return zwp_linux_buffer_params_v1_create_immed(dmabuf_params,
		width, height, drm_format, 0);
}

static void handle_buffer_release(void *data, struct wl_buffer *buffer)
{
	AVFrame *frame = data;
	av_frame_free(&frame);

	wl_buffer_destroy(buffer);
}

static const struct wl_buffer_listener buffer_listener = {
	.release = handle_buffer_release,
};

static void on_resize(GtkWidget* widget, gint width, gint height, struct ATL_codec_context *ctx)
{
	ctx->video.surface_width = gtk_widget_get_width(widget);
	ctx->video.surface_height = gtk_widget_get_height(widget);
	wp_viewport_set_destination(ctx->video.viewport, ctx->video.surface_width, ctx->video.surface_height);
}

JNIEXPORT void JNICALL Java_android_media_MediaCodec_native_1configure_1video(JNIEnv *env, jobject this, jlong codec, jobject csd0, jobject csd1, jobject surface_obj)
{
	struct ATL_codec_context *ctx = _PTR(codec);
	AVCodecContext *codec_ctx = ctx->codec;
	jarray array_ref;
	jbyte *array;
	void *data;
	int sps_size;
	int pps_size;

	printf("Java_android_media_MediaCodec_native_1configure_video(%s)\n", codec_ctx->codec->name);

	sps_size = get_nio_buffer_size(env, csd0);
	pps_size = get_nio_buffer_size(env, csd1);

	// libavcodec wants the complete AVC decoder configuration record, but android APIs pass only SPS and PPS records
	// see https://stackoverflow.com/questions/29790334/how-to-fill-extradata-field-of-avcodeccontext-with-sps-and-pps-data
	codec_ctx->extradata_size = 11 + sps_size + pps_size;
	codec_ctx->extradata = av_mallocz(codec_ctx->extradata_size + AV_INPUT_BUFFER_PADDING_SIZE);
	codec_ctx->extradata[0] = 0x01;
	codec_ctx->extradata[1] = 0x4d;
	codec_ctx->extradata[2] = 0x40;
	codec_ctx->extradata[3] = 0x1f;
	codec_ctx->extradata[4] = 0xff;
	codec_ctx->extradata[5] = 0xe0 | 1;
	codec_ctx->extradata[6] = (sps_size>>8) & 0xff;
	codec_ctx->extradata[7] = (sps_size) & 0xff;
	data = get_nio_buffer(env, csd0, &array_ref, &array);
	memcpy(codec_ctx->extradata + 8, data, sps_size);
	release_nio_buffer(env, array_ref, array);
	codec_ctx->extradata[8 + sps_size] = 1;
	codec_ctx->extradata[9 + sps_size] = (pps_size>>8) & 0xff;
	codec_ctx->extradata[10 + sps_size] = (pps_size) & 0xff;
	data = get_nio_buffer(env, csd1, &array_ref, &array);
	memcpy(codec_ctx->extradata + 11 + sps_size, data, pps_size);
	release_nio_buffer(env, array_ref, array);

	for (int i = 0; i < codec_ctx->extradata_size; i++) {
		printf("params->extradata[%d] = %x\n", i, codec_ctx->extradata[i]);
	}

	enum AVHWDeviceType type = AV_HWDEVICE_TYPE_VAAPI;
	int ret = check_hw_device_type(type);
	if (ret != 0) {
		fprintf(stderr, "VA-API not supported\n");
		exit(1);
	}
	int i = 0;
	while (1) {
		const AVCodecHWConfig *config = avcodec_get_hw_config(codec_ctx->codec, i);
		if (!config) {
			fprintf(stderr, "Decoder %s doesn't support device type %s\n",
				codec_ctx->codec->name, av_hwdevice_get_type_name(type));
			exit(1);
		}

		if ((config->methods & AV_CODEC_HW_CONFIG_METHOD_HW_DEVICE_CTX) &&
				config->device_type == type) {
			hw_pix_fmt = config->pix_fmt;
			break;
		}

		i++;
	}
	fprintf(stderr, "Selected pixel format %s\n", av_get_pix_fmt_name(hw_pix_fmt));

	struct ANativeWindow *native_window = ANativeWindow_fromSurface(env, surface_obj);
	ctx->video.native_window = native_window;
	ctx->video.surface_width = gtk_widget_get_width(native_window->surface_view_widget);
	ctx->video.surface_height = gtk_widget_get_height(native_window->surface_view_widget);
}

JNIEXPORT void JNICALL Java_android_media_MediaCodec_native_1start(JNIEnv *env, jobject this, jlong codec)
{
	struct ATL_codec_context *ctx = _PTR(codec);
	AVCodecContext *codec_ctx = ctx->codec;

	if(avcodec_open2(codec_ctx, codec_ctx->codec, NULL)<0){
		printf("Codec cannot be found");
	}

	if (codec_ctx->codec_type == AVMEDIA_TYPE_AUDIO) {
		printf("ctx->sample_rate = %d\n", codec_ctx->sample_rate);
		printf("ctx->ch_layout.nb_channels = %d\n", codec_ctx->ch_layout.nb_channels);
		printf("ctx->sample_fmt = %d\n", codec_ctx->sample_fmt);

		int ret = swr_alloc_set_opts2(&ctx->audio.swr,
			&codec_ctx->ch_layout,
			AV_SAMPLE_FMT_S16,
			codec_ctx->sample_rate,
			&codec_ctx->ch_layout,
			codec_ctx->sample_fmt,
			codec_ctx->sample_rate,
			0,
			NULL);
		if (ret != 0) {
			fprintf(stderr, "FFmpegDecoder error: Swresampler alloc fail\n");
		}
		swr_init(ctx->audio.swr);
	} else if (codec_ctx->codec_type == AVMEDIA_TYPE_VIDEO) {
		enum AVHWDeviceType type = AV_HWDEVICE_TYPE_VAAPI;
		codec_ctx->get_format = get_hw_format;

		AVBufferRef *hw_device_ctx = NULL;
		int ret = av_hwdevice_ctx_create(&hw_device_ctx, type, NULL, NULL, 0);
		if (ret < 0) {
			fprintf(stderr, "Failed to create HW device context\n");
			exit(1);
		}
		codec_ctx->hw_device_ctx = av_buffer_ref(hw_device_ctx);

		struct ANativeWindow *native_window = ctx->video.native_window;
		struct wl_registry *registry = wl_display_get_registry(native_window->wayland_display);
		wl_registry_add_listener(registry, &registry_listener, ctx);
		wl_display_roundtrip(native_window->wayland_display);
		wl_registry_destroy(registry);

		if (ctx->video.zwp_linux_dmabuf_v1 == NULL || ctx->video.wp_viewporter == NULL) {
			fprintf(stderr, "Missing zwp_linux_dmabuf_v1 or wp_viewporter support\n");
			exit(1);
		}

		ctx->video.viewport = wp_viewporter_get_viewport(ctx->video.wp_viewporter, native_window->wayland_surface);
		wp_viewport_set_destination(ctx->video.viewport, ctx->video.surface_width, ctx->video.surface_height);
		g_signal_connect(native_window->surface_view_widget, "resize", G_CALLBACK(on_resize), ctx);
	}
}

#define INFO_TRY_AGAIN_LATER -1

JNIEXPORT jint JNICALL Java_android_media_MediaCodec_native_1queueInputBuffer(JNIEnv *env, jobject this, jlong codec, jobject buffer, jlong presentationTimeUs)
{
	jarray array_ref;
	jbyte *array;
	int ret;
	struct ATL_codec_context *ctx = _PTR(codec);
	AVCodecContext *codec_ctx = ctx->codec;
	AVPacket *pkt = av_packet_alloc();

	pkt->size = get_nio_buffer_size(env, buffer);
	pkt->data = get_nio_buffer(env, buffer, &array_ref, &array);
	pkt->pts = presentationTimeUs;
	ret = avcodec_send_packet(codec_ctx, pkt);
	if (ret < 0 && ret != AVERROR(EAGAIN)) {
		fprintf(stderr, "Error while sending packet: %d = %s\n", ret, av_err2str(ret));
	}
	release_nio_buffer(env, array_ref, array);
	av_packet_free(&pkt);
	return ret;
}

JNIEXPORT jint JNICALL Java_android_media_MediaCodec_native_1dequeueOutputBuffer(JNIEnv *env, jobject this, jlong codec, jobject buffer, jobject buffer_info)
{
	struct ATL_codec_context *ctx = _PTR(codec);
	AVCodecContext *codec_ctx = ctx->codec;
	AVFrame *frame = av_frame_alloc();
	int ret;
	jarray array_ref;
	jbyte *array;

	ret = avcodec_receive_frame(codec_ctx, frame);
	if (ret < 0) {
		if (ret != AVERROR(EAGAIN)) {
			printf("avcodec_receive_frame returned %d\n", ret);
			printf("frame->data = %p frame->nb_samples = %d\n", frame->data[0], frame->nb_samples);
		}
		av_frame_free(&frame);
		return INFO_TRY_AGAIN_LATER;
	}
	_SET_LONG_FIELD(buffer_info, "presentationTimeUs", frame->pts);

	uint8_t *raw_buffer = get_nio_buffer(env, buffer, &array_ref, &array);
	if (codec_ctx->codec_type == AVMEDIA_TYPE_AUDIO) {
		int outSamples = swr_convert(ctx->audio.swr, &raw_buffer, frame->nb_samples, (uint8_t const **) (frame->data), frame->nb_samples);
		_SET_INT_FIELD(buffer_info, "offset", 0);
		_SET_INT_FIELD(buffer_info, "size", outSamples * 2 * 2);

		av_frame_free(&frame);
	} else if (codec_ctx->codec_type == AVMEDIA_TYPE_VIDEO) {
		// copy frame pointer into data buffer to be read by releaseOutputBuffer function
		*((AVFrame **)raw_buffer) = frame;
		_SET_INT_FIELD(buffer_info, "offset", 0);
		_SET_INT_FIELD(buffer_info, "size", sizeof(AVFrame *));
	}
	release_nio_buffer(env, array_ref, array);
	return 0;
}

// callback to perform wayland stuff on main thread
struct render_frame_data {AVFrame *frame; struct ATL_codec_context *ctx;};

static gboolean render_frame(void *data)
{
	struct render_frame_data *d = (struct render_frame_data *)data;
	AVFrame *frame = d->frame;
	struct ATL_codec_context *ctx = d->ctx;
	int ret;

	AVFrame *drm_frame = av_frame_alloc();
	drm_frame->format = AV_PIX_FMT_DRM_PRIME;
	drm_frame->hw_frames_ctx = av_buffer_ref(frame->hw_frames_ctx);

	// Convert the VA-API frame into a DMA-BUF frame
	ret = av_hwframe_map(drm_frame, frame, 0);
	if (ret < 0) {
		fprintf(stderr, "Failed to map frame: %s\n", av_err2str(ret));
		exit(1);
	}

	AVDRMFrameDescriptor *drm_frame_desc = (void *)drm_frame->data[0];

	struct wl_buffer *wl_buffer = import_drm_frame_desc(ctx->video.zwp_linux_dmabuf_v1,
		drm_frame_desc, drm_frame->width, drm_frame->height);
	if (!wl_buffer) {
		exit(1);
	}
	wl_buffer_add_listener(wl_buffer, &buffer_listener, frame);

	struct ANativeWindow *native_window = ctx->video.native_window;

	wl_surface_damage(native_window->wayland_surface, 0, 0, INT32_MAX, INT32_MAX);
	wl_surface_attach(native_window->wayland_surface, wl_buffer, 0, 0);
	wl_surface_commit(native_window->wayland_surface);

	// actual frame will be freed in handle_buffer_release callback
	av_frame_free(&drm_frame);
	free(d);

	return G_SOURCE_REMOVE;
}

JNIEXPORT void JNICALL Java_android_media_MediaCodec_native_1releaseOutputBuffer(JNIEnv *env, jobject this, jlong codec, jobject buffer, jboolean render)
{
	struct ATL_codec_context *ctx = _PTR(codec);
	jarray array_ref;
	jbyte *array;
	AVFrame *frame;

	if (ctx->codec->codec_type == AVMEDIA_TYPE_VIDEO) {
		AVFrame **raw_buffer = get_nio_buffer(env, buffer, &array_ref, &array);
		frame = *raw_buffer;
		release_nio_buffer(env, array_ref, array);

		if (!render) {
			fprintf(stderr, "skipping %dx%d frame!\n", frame->width, frame->height);
			av_frame_free(&frame);
			return;
		}

		struct render_frame_data *data = malloc(sizeof(struct render_frame_data));
		data->frame = frame;
		data->ctx = ctx;
		g_idle_add(render_frame, data);
	}
}
