#include <gtk/gtk.h>

#include <alsa/asoundlib.h>
#include <stdint.h>
#include <stdio.h>

#include "../defines.h"
#include "../util.h"

#include "../generated_headers/android_media_AudioTrack.h"

//#define PCM_DEVICE "sysdefault:CARD=Generic_1"
#define PCM_DEVICE getenv("HAX_AUDIOTRACK_PCM_DEVICE")

void helper_hw_params_init(snd_pcm_t *pcm_handle, snd_pcm_hw_params_t *params, unsigned int rate, unsigned int channels, snd_pcm_format_t format)
{
	int ret;

	snd_pcm_hw_params_any(pcm_handle, params);

	ret = snd_pcm_hw_params_set_access(pcm_handle, params, SND_PCM_ACCESS_RW_INTERLEAVED);
	if (ret < 0)
		printf("ERROR: Can't set interleaved mode. %s\n", snd_strerror(ret));

	ret = snd_pcm_hw_params_set_format(pcm_handle, params, format);
	if (ret < 0)
		printf("ERROR: Can't set format. %s\n", snd_strerror(ret));

	ret = snd_pcm_hw_params_set_channels(pcm_handle, params, channels);
	if (ret < 0)
		printf("ERROR: Can't set channels number. %s\n", snd_strerror(ret));

	ret = snd_pcm_hw_params_set_rate_near(pcm_handle, params, &rate, 0);
	if (ret < 0)
		printf("ERROR: Can't set rate. %s\n", snd_strerror(ret));
}

JNIEXPORT void JNICALL Java_android_media_AudioTrack_native_1constructor(JNIEnv *env, jobject this, jint streamType, jint rate, jint channels, jint audioFormat, jint buffer_size, jint mode)
{


	snd_pcm_t *pcm_handle;
	snd_pcm_hw_params_t *params;

	unsigned int channels_out;
	unsigned int period_time;

	int ret;

	if(!PCM_DEVICE)
		return; // STUB

	/* Open the PCM device in playback mode */
	ret = snd_pcm_open(&pcm_handle, PCM_DEVICE, SND_PCM_STREAM_PLAYBACK, 0);
	if (ret < 0)
		printf("ERROR: Can't open \"%s\" PCM device. %s\n", PCM_DEVICE, snd_strerror(ret));

	snd_pcm_hw_params_alloca(&params);
	helper_hw_params_init(pcm_handle, params, rate, channels, SND_PCM_FORMAT_S16_LE);

/*--↓*/
	snd_pcm_uframes_t buffer_size_as_uframes_t = buffer_size;
	snd_pcm_hw_params_set_buffer_size_near (pcm_handle, params, &buffer_size_as_uframes_t);
/*--↑*/

	/* Write parameters */
	ret = snd_pcm_hw_params(pcm_handle, params);
	if (ret < 0)
		printf("ERROR: Can't set harware parameters. %s\n", snd_strerror(ret));

	//snd_pcm_hw_params_free (hw_params);

/*--↓*/
	snd_pcm_uframes_t period_size;

	ret = snd_pcm_hw_params_get_period_size(params, &period_size, 0);
	if (ret < 0)
		printf("Error calling snd_pcm_hw_params_get_period_size: %s\n", snd_strerror(ret));

	snd_pcm_sw_params_t *sw_params;

	snd_pcm_sw_params_malloc (&sw_params);
	snd_pcm_sw_params_current (pcm_handle, sw_params);

	snd_pcm_sw_params_set_start_threshold(pcm_handle, sw_params, buffer_size - period_size);
	snd_pcm_sw_params_set_avail_min(pcm_handle, sw_params, period_size);

	snd_pcm_sw_params(pcm_handle, sw_params);

	//snd_pcm_sw_params_free (sw_params);
/*--↑*/

	/* Resume information */
	printf("PCM name: '%s'\n", snd_pcm_name(pcm_handle));

	printf("PCM state: %s\n", snd_pcm_state_name(snd_pcm_state(pcm_handle)));

	snd_pcm_hw_params_get_channels(params, &channels_out);
	printf("channels: %i ", channels_out);

	if (channels_out == 1)
		printf("(mono)\n");
	else if (channels_out == 2)
		printf("(stereo)\n");

	unsigned int tmp;
	snd_pcm_hw_params_get_rate(params, &tmp, 0);
	printf("rate: %d bps\n", tmp);

	snd_pcm_hw_params_get_period_time(params, &period_time, NULL);

	_SET_LONG_FIELD(this, "pcm_handle", _INTPTR(pcm_handle));
	_SET_LONG_FIELD(this, "params", _INTPTR(params));
	_SET_INT_FIELD(this, "channels", channels_out);
	_SET_INT_FIELD(this, "period_time", period_time);
}

JNIEXPORT jint JNICALL Java_android_media_AudioTrack_getMinBufferSize(JNIEnv *env, jclass this_class, jint sampleRateInHz, jint channelConfig, jint audioFormat)
{
	snd_pcm_t *pcm_handle;
	snd_pcm_hw_params_t *params;
	snd_pcm_uframes_t frames;
	int ret;

	// TODO: clean up
	unsigned int num_channels;
	switch(channelConfig) {
		case 2:
			num_channels = 1;
			break;
		default:
			num_channels = 1;
	}
	// ---

	if(!PCM_DEVICE)
		return 256; // STUB

	ret = snd_pcm_open(&pcm_handle, PCM_DEVICE, SND_PCM_STREAM_PLAYBACK, 0);
	if (ret < 0)
		printf("Error calling snd_pcm_open: %s\n", snd_strerror(ret));

	snd_pcm_hw_params_alloca(&params);
	helper_hw_params_init(pcm_handle, params, sampleRateInHz, num_channels, SND_PCM_FORMAT_S16_LE); // FIXME: a switch?

	ret = snd_pcm_hw_params(pcm_handle, params);
	if (ret < 0)
		printf("Error calling snd_pcm_hw_params: %s\n", snd_strerror(ret));

	ret = snd_pcm_hw_params_get_period_size(params, &frames, 0);
	if (ret < 0)
		printf("Error calling snd_pcm_hw_params_get_period_size: %s\n", snd_strerror(ret));

// TODO: snd_pcm_hw_params_free(params) causes segfault, is it not supposed to be called?
	snd_pcm_close(pcm_handle);

	_SET_STATIC_INT_FIELD(this_class, "frames", frames);
	if((*env)->ExceptionCheck(env))
		(*env)->ExceptionDescribe(env);

	printf("\n\nJava_android_media_AudioTrack_getMinBufferSize is returning: %ld\n\n\n", frames * num_channels * 2);
	return frames * num_channels * 2; // FIXME: 2 bytes = 16 bits (s16)
}

struct jni_callback_data { JavaVM *jvm; jobject this; jclass this_class; jobject listener; jint period_time;};
void periodic_update_callback(snd_async_handler_t *pcm_callback)
{
	struct jni_callback_data *d = snd_async_handler_get_callback_private(pcm_callback);
	int getenv_ret;
	int attach_ret = -1;

	if(!PCM_DEVICE)
		return; // STUB

//	printf("periodic_update_callback called!\n");

	JNIEnv *env;
	getenv_ret = (*d->jvm)->GetEnv(d->jvm, (void**)&env, JNI_VERSION_1_6);

//	printf("!!!! GetEnv: %p getenv_ret: %d\n",env, getenv_ret);
	if(getenv_ret == JNI_EDETACHED) {
		printf("!!!! JNI_EDETACHED\n");
		attach_ret = (*d->jvm)->AttachCurrentThread(d->jvm, (void**)&env, NULL);
		// TODO error checking
	}

	(*env)->CallVoidMethod(env, d->listener, handle_cache.audio_track_periodic_listener.onPeriodicNotification, d->this);

	if((*env)->ExceptionCheck(env))
		(*env)->ExceptionDescribe(env);

	if(attach_ret == JNI_OK) // if we (succesfully) attached a thread, we should probably detach it now
		(*d->jvm)->DetachCurrentThread(d->jvm);

	// microseconds to milliseconds
//	g_timeout_add (d->period_time / 1000 - 2, G_SOURCE_FUNC(helper_loop), d);
//	return G_SOURCE_REMOVE;
}

JNIEXPORT void JNICALL Java_android_media_AudioTrack_native_1play(JNIEnv *env, jobject this)
{
	jint period_time = _GET_INT_FIELD(this, "period_time");

	// FIXME - this callback should probably be set up elsewhere

	JavaVM *jvm;
	(*env)->GetJavaVM(env, &jvm);

	struct jni_callback_data *callback_data = malloc(sizeof(struct jni_callback_data));
	callback_data->jvm = jvm;
	callback_data->this = _REF(this);
	callback_data->this_class = _REF(_CLASS(this));
	callback_data->listener = _REF(_GET_OBJ_FIELD(this, "periodic_update_listener", "Landroid/media/AudioTrack$OnPlaybackPositionUpdateListener;"));
	callback_data->period_time = period_time;

	// microseconds to milliseconds
	//g_timeout_add (period_time / 1000, G_SOURCE_FUNC(helper_loop), callback_data);

/*--↓*/
	snd_pcm_t *pcm_handle = _PTR(_GET_LONG_FIELD(this, "pcm_handle"));

	snd_async_handler_t *pcm_callback;

	snd_async_add_pcm_handler(&pcm_callback, pcm_handle, periodic_update_callback, callback_data);
	snd_pcm_start(pcm_handle);
/*--↑*/
}

JNIEXPORT jint JNICALL Java_android_media_AudioTrack_native_1write(JNIEnv *env, jobject this, jbyteArray audioData, jint offsetInBytes, jint frames_to_write)
{
	int ret;

	if(!PCM_DEVICE)
		return 0; // STUB

	snd_pcm_t *pcm_handle = _PTR(_GET_LONG_FIELD(this, "pcm_handle"));

	snd_pcm_sframes_t frames_written;

	jbyte *buffer = _GET_BYTE_ARRAY_ELEMENTS(audioData);

	ret = frames_written = snd_pcm_writei(pcm_handle, buffer, frames_to_write);
	if (ret < 0) {
		if (ret == -EPIPE) {
			printf("XRUN.\n");
			snd_pcm_prepare(pcm_handle);
		} else {
			printf("ERROR. Can't write to PCM device. %s\n", snd_strerror(ret));
		}
	}

//	printf("::::> tried to write %d frames, actually wrote %d frames.\n", frames_to_write, frames_written);

	_RELEASE_BYTE_ARRAY_ELEMENTS(audioData, buffer);
	return frames_written;
}
