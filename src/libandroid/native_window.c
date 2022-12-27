/*
 * Copyright (C) 2010 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License")
{
	return -1;
}
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/**
 * @addtogroup NativeActivity Native Activity
 * @{
 */

/**
 * @file native_window.h
 * @brief API for accessing a native window.
 */


#include <stdint.h>
#include <stdlib.h>

#include <EGL/egl.h>
#include <EGL/eglext.h>

#include <wayland-client.h>
#include <wayland-egl.h>

#include <gtk/gtk.h>
#include <gdk/wayland/gdkwayland.h>

// FIXME: move this together with the other stuff that doesn't belong in this file
#include <openxr/openxr.h>
#define XR_USE_PLATFORM_EGL
#include <openxr/openxr_platform.h>

#include <jni.h>

// FIXME: put the header in a common place
#include "../api-impl-jni/defines.h"

/**
 * Transforms that can be applied to buffers as they are displayed to a window.
 *
 * Supported transforms are any combination of horizontal mirror, vertical
 * mirror, and clockwise 90 degree rotation, in that order. Rotations of 180
 * and 270 degrees are made up of those basic transforms.
 */
enum ANativeWindowTransform {
    ANATIVEWINDOW_TRANSFORM_IDENTITY            = 0x00,
    ANATIVEWINDOW_TRANSFORM_MIRROR_HORIZONTAL   = 0x01,
    ANATIVEWINDOW_TRANSFORM_MIRROR_VERTICAL     = 0x02,
    ANATIVEWINDOW_TRANSFORM_ROTATE_90           = 0x04,

    ANATIVEWINDOW_TRANSFORM_ROTATE_180          = ANATIVEWINDOW_TRANSFORM_MIRROR_HORIZONTAL |
                                                  ANATIVEWINDOW_TRANSFORM_MIRROR_VERTICAL,
    ANATIVEWINDOW_TRANSFORM_ROTATE_270          = ANATIVEWINDOW_TRANSFORM_ROTATE_180 |
                                                  ANATIVEWINDOW_TRANSFORM_ROTATE_90,
};

struct ANativeWindow {
	EGLNativeWindowType egl_window;
	GtkWidget *surface_view_widget;
};

/**
 * Opaque type that provides access to a native window.
 *
 * A pointer can be obtained using {@link ANativeWindow_fromSurface()}.
 */
typedef struct ANativeWindow ANativeWindow;

/**
 * Struct that represents a windows buffer.
 *
 * A pointer can be obtained using {@link ANativeWindow_lock()}.
 */
typedef struct ANativeWindow_Buffer {
    // The number of pixels that are show horizontally.
    int32_t width;

    // The number of pixels that are shown vertically.
    int32_t height;

    // The number of *pixels* that a line in the buffer takes in
    // memory. This may be >= width.
    int32_t stride;

    // The format of the buffer. One of AHARDWAREBUFFER_FORMAT_*
    int32_t format;

    // The actual bits.
    void* bits;

    // Do not touch.
    uint32_t reserved[6];
} ANativeWindow_Buffer;

/**
 * Acquire a reference on the given {@link ANativeWindow} object. This prevents the object
 * from being deleted until the reference is removed.
 */
void ANativeWindow_acquire(struct ANativeWindow *native_window)
{
	// FIXME: refcount
}

void ANativeWindow_release(struct ANativeWindow *native_window)
{
	// FIXME: refcount
}

int32_t ANativeWindow_getWidth(struct ANativeWindow *native_window)
{
	return gtk_widget_get_width(native_window->surface_view_widget);
}

int32_t ANativeWindow_getHeight(struct ANativeWindow *native_window)
{
	return gtk_widget_get_height(native_window->surface_view_widget);
}

/**
 * Return the current pixel format (AHARDWAREBUFFER_FORMAT_*) of the window surface.
 *
 * \return a negative value on error.
 */
int32_t ANativeWindow_getFormat(ANativeWindow* window)
{
	return -1;
}

/**
 * Change the format and size of the window buffers.
 *
 * The width and height control the number of pixels in the buffers, not the
 * dimensions of the window on screen. If these are different than the
 * window's physical size, then its buffer will be scaled to match that size
 * when compositing it to the screen. The width and height must be either both zero
 * or both non-zero.
 *
 * For all of these parameters, if 0 is supplied then the window's base
 * value will come back in force.
 *
 * \param width width of the buffers in pixels.
 * \param height height of the buffers in pixels.
 * \param format one of AHARDWAREBUFFER_FORMAT_* constants.
 * \return 0 for success, or a negative value on error.
 */
int32_t ANativeWindow_setBuffersGeometry(ANativeWindow* window,
        int32_t width, int32_t height, int32_t format)
{
	return -1;
}

/**
 * Lock the window's next drawing surface for writing.
 * inOutDirtyBounds is used as an in/out parameter, upon entering the
 * function, it contains the dirty region, that is, the region the caller
 * intends to redraw. When the function returns, inOutDirtyBounds is updated
 * with the actual area the caller needs to redraw -- this region is often
 * extended by {@link ANativeWindow_lock}.
 *
 * \return 0 for success, or a negative value on error.
 */

typedef void ARect;

int32_t ANativeWindow_lock(ANativeWindow* window, ANativeWindow_Buffer* outBuffer,
        ARect* inOutDirtyBounds)
{
	return -1;
}

/**
 * Unlock the window's drawing surface after previously locking it,
 * posting the new buffer to the display.
 *
 * \return 0 for success, or a negative value on error.
 */
int32_t ANativeWindow_unlockAndPost(ANativeWindow* window)
{
	return -1;
}

int32_t ANativeWindow_setFrameRate(ANativeWindow *window, float frameRate, int8_t compatibility)
{
	return 0; // success
}

/**
 * Set a transform that will be applied to future buffers posted to the window.
 *
 * \param transform combination of {@link ANativeWindowTransform} flags
 * \return 0 for success, or -EINVAL if \p transform is invalid
 */
int32_t ANativeWindow_setBuffersTransform(ANativeWindow* window, int32_t transform)
{
	return -1;
}

void wl_registry_global_handler(void *data, struct wl_registry *registry, uint32_t name, const char *interface, uint32_t version)
{
	struct wl_subcompositor **subcompositor = data;
	printf("interface: '%s', version: %u, name: %u\n", interface, version, name);
	if (!strcmp(interface, "wl_subcompositor")) {
		*subcompositor = wl_registry_bind(registry, name, &wl_subcompositor_interface, 1);
	}
}

void wl_registry_global_remove_handler(void *data, struct wl_registry *registry, uint32_t name)
{
	printf("removed: %u\n", name);
}

// TODO: handle X11
static void on_resize(GtkWidget* self, gint width, gint height, struct wl_egl_window *egl_window)
{
	wl_egl_window_resize(egl_window, width, height, 0, 0);
}

ANativeWindow * ANativeWindow_fromSurface(JNIEnv* env, jobject surface)
{
	int width;
	int height;

	double pos_x;
	double pos_y;
	double off_x;
	double off_y;

	GtkWidget *surface_view_widget = _PTR(_GET_LONG_FIELD(surface, "widget"));
	GtkWidget *window = GTK_WIDGET(gtk_widget_get_native(surface_view_widget));
	while( (width = gtk_widget_get_width(surface_view_widget)) == 0 ) {
		// FIXME: UGLY: this loop waits until the SurfaceView widget gets mapped
	}
	height = gtk_widget_get_height(surface_view_widget);


	// get position of the SurfaceView widget wrt the toplevel window
	gtk_widget_translate_coordinates(surface_view_widget, window, 0, 0, &pos_x, &pos_y);
	// compensate for offset between the widget coordinates and the surface coordinates
	gtk_native_get_surface_transform(GTK_NATIVE(window), &off_x, &off_y);
	pos_x += off_x;
	pos_y += off_y;

	printf("XXXXX: SurfaceView widget: %p (%s), width: %d, height: %d\n", surface_view_widget, gtk_widget_get_name(surface_view_widget), width, height);
	printf("XXXXX: SurfaceView widget: x: %lf, y: %lf\n", pos_x, pos_y);
	printf("XXXXX: native offset: x: %lf, y: %lf\n", off_x, off_y);

	struct ANativeWindow *native_window = malloc(sizeof(struct ANativeWindow));
	native_window->surface_view_widget = surface_view_widget;

	GdkDisplay *display = gtk_root_get_display(GTK_ROOT(window));
	// FIXME: add a path for x11
	// start of wayland-specific code
	struct wl_display *wl_display = gdk_wayland_display_get_wl_display(display);
	struct wl_compositor *wl_compositor = gdk_wayland_display_get_wl_compositor(display);

	struct wl_registry *wl_registry = wl_display_get_registry(wl_display);
	struct wl_registry_listener wl_registry_listener = {
		.global = wl_registry_global_handler,
		.global_remove = wl_registry_global_remove_handler
	};
	struct wl_subcompositor *wl_subcompositor = NULL;
	wl_registry_add_listener(wl_registry, &wl_registry_listener, &wl_subcompositor);
	wl_display_roundtrip(wl_display);
	printf("XXX: wl_subcompositor: %p\n", wl_subcompositor);

	struct wl_surface *toplevel_surface = gdk_wayland_surface_get_wl_surface(gtk_native_get_surface(GTK_NATIVE(window)));

	struct wl_surface *wayland_surface = wl_compositor_create_surface(wl_compositor);

	struct wl_subsurface *subsurface = wl_subcompositor_get_subsurface(wl_subcompositor, wayland_surface, toplevel_surface);
	wl_subsurface_set_desync(subsurface);
	wl_subsurface_set_position(subsurface, pos_x, pos_y);

	struct wl_region *empty_region = wl_compositor_create_region(wl_compositor);
	wl_surface_set_input_region(wayland_surface, empty_region);
	wl_region_destroy(empty_region);

	struct wl_egl_window *egl_window = wl_egl_window_create(wayland_surface, width, height);
	native_window->egl_window = (EGLNativeWindowType)egl_window;
	printf("EGL::: wayland_surface: %p\n", wayland_surface);
	// end of wayland-specific code
	printf("EGL::: native_window->egl_window: %ld\n", native_window->egl_window);

	g_signal_connect(surface_view_widget, "resize", G_CALLBACK(on_resize), egl_window);

	return native_window;
}

ANativeWindow * ANativeWindow_fromSurfaceTexture(JNIEnv* env, jobject surfaceTexture)
{
	return NULL;
}

// temporary for debugging
static void PrintConfigAttributes(EGLDisplay display, EGLConfig config)
{
	EGLint value;
	printf("-------------------------------------------------------------------------------\n");
	eglGetConfigAttrib(display,config,EGL_CONFIG_ID,&value);
	printf("EGL_CONFIG_ID %d\n",value);

	eglGetConfigAttrib(display,config,EGL_BUFFER_SIZE,&value);
	printf("EGL_BUFFER_SIZE %d\n",value);
	eglGetConfigAttrib(display,config,EGL_RED_SIZE,&value);
	printf("EGL_RED_SIZE %d\n",value);
	eglGetConfigAttrib(display,config,EGL_GREEN_SIZE,&value);
	printf("EGL_GREEN_SIZE %d\n",value);
	eglGetConfigAttrib(display,config,EGL_BLUE_SIZE,&value);
	printf("EGL_BLUE_SIZE %d\n",value);
	eglGetConfigAttrib(display,config,EGL_ALPHA_SIZE,&value);
	printf("EGL_ALPHA_SIZE %d\n",value);
	eglGetConfigAttrib(display,config,EGL_DEPTH_SIZE,&value);
	printf("EGL_DEPTH_SIZE %d\n",value);
	eglGetConfigAttrib(display,config,EGL_STENCIL_SIZE,&value);
	printf("EGL_STENCIL_SIZE %d\n",value);
	eglGetConfigAttrib(display,config,EGL_SAMPLE_BUFFERS,&value);
	printf("EGL_SAMPLE_BUFFERS %d\n",value);
	eglGetConfigAttrib(display,config,EGL_SAMPLES,&value);
	printf("EGL_SAMPLES %d\n",value);

	eglGetConfigAttrib(display,config,EGL_CONFIG_CAVEAT,&value);
	switch(value)
	{
		case EGL_NONE:
			printf("EGL_CONFIG_CAVEAT EGL_NONE\n");
			break;
		case EGL_SLOW_CONFIG:
			printf("EGL_CONFIG_CAVEAT EGL_SLOW_CONFIG\n");
			break;
	}

	eglGetConfigAttrib(display,config,EGL_MAX_PBUFFER_WIDTH,&value);
	printf("EGL_MAX_PBUFFER_WIDTH %d\n",value);
	eglGetConfigAttrib(display,config,EGL_MAX_PBUFFER_HEIGHT,&value);
	printf("EGL_MAX_PBUFFER_HEIGHT %d\n",value);
	eglGetConfigAttrib(display,config,EGL_MAX_PBUFFER_PIXELS,&value);
	printf("EGL_MAX_PBUFFER_PIXELS %d\n",value);
	eglGetConfigAttrib(display,config,EGL_NATIVE_RENDERABLE,&value);
	printf("EGL_NATIVE_RENDERABLE %s \n",(value ? "true" : "false"));
	eglGetConfigAttrib(display,config,EGL_NATIVE_VISUAL_ID,&value);
	printf("EGL_NATIVE_VISUAL_ID %d\n",value);
	eglGetConfigAttrib(display,config,EGL_NATIVE_VISUAL_TYPE,&value);
	printf("EGL_NATIVE_VISUAL_TYPE %d\n",value);
	eglGetConfigAttrib(display,config,EGL_RENDERABLE_TYPE,&value);
	printf("EGL_RENDERABLE_TYPE %d\n",value);
	eglGetConfigAttrib(display,config,EGL_SURFACE_TYPE,&value);
	printf("EGL_SURFACE_TYPE %d\n",value);
	eglGetConfigAttrib(display,config,EGL_TRANSPARENT_TYPE,&value);
	printf("EGL_TRANSPARENT_TYPE %d\n",value);
	printf("-------------------------------------------------------------------------------\n");
}

// FIXME: this possibly belongs elsewhere

extern GtkWindow *window; // TODO: how do we get rid of this? the app won't pass anyhting useful to eglGetDisplay

// this is an extension that only android implements, we can hopefully get away with just stubbing it
EGLBoolean bionic_eglPresentationTimeANDROID(EGLDisplay dpy, EGLSurface surface, EGLnsecsANDROID time)
{
	return EGL_TRUE;
}

EGLDisplay bionic_eglGetDisplay(NativeDisplayType native_display)
{
	// XXX - we can't use pbuffers with wayland EGLDisplay, for now one has to use an env to bypass
	// our EGLDisplay substitution in VR usecases (the XWayland display does support pbuffers, but
	// is different from the display the SurfaceView is on, which means trying to draw to that will
	// not work)
	if(getenv("UGLY_HACK_FOR_VR")) {
		return eglGetDisplay(native_display);
	} else {
		/*
		 * On android, at least SDL passes 0 (EGL_DISPLAY_DEFAULT) to eglGetDisplay and uses the resulting display.
		 * We obviously want to make the app use the correct display, which may happen to be a different one
		 * than the "default" display (especially on Wayland)
		 */
		GdkDisplay *display = gtk_root_get_display(GTK_ROOT(window));
		struct wl_display *wl_display = gdk_wayland_display_get_wl_display(display);

		return eglGetPlatformDisplay(EGL_PLATFORM_WAYLAND_KHR, wl_display, NULL);
	}
}

EGLSurface bionic_eglCreateWindowSurface(EGLDisplay display, EGLConfig config, struct ANativeWindow *native_window, EGLint const *attrib_list)
{
	// better than crashing (TODO: check if apps try to use the NULL value anyway)
	if(!native_window)
		return NULL;

	PrintConfigAttributes(display, config);
	EGLSurface ret = eglCreateWindowSurface(display, config, native_window->egl_window, attrib_list);

	printf("EGL::: native_window->egl_window: %ld\n", native_window->egl_window);
	printf("EGL::: eglGetError: %d\n", eglGetError());

	printf("EGL::: ret: %p\n", ret);

	return ret;
}

// FIXME 2: this BLATANTLY belongs elsewhere

static void xrInitializeLoaderKHR_noop() //FIXME: does it really return void?
{
	printf("STUB: xrInitializeLoaderKHR_noop called\n");
}

struct XrGraphicsBindingOpenGLESAndroidKHR {
    XrStructureType	type;
    const void* next;
    EGLDisplay display;
    EGLConfig config;
    EGLContext context;
};

XrResult bionic_xrCreateSession(XrInstance instance, XrSessionCreateInfo *createInfo, XrSession *session)
{
	// TODO: check the type and handle Vulkan
	struct XrGraphicsBindingOpenGLESAndroidKHR *android_bind = createInfo->next;

	XrGraphicsBindingEGLMNDX egl_bind = {XR_TYPE_GRAPHICS_BINDING_EGL_MNDX};
	egl_bind.getProcAddress = eglGetProcAddress;
	egl_bind.display = android_bind->display;
	egl_bind.config = android_bind->config;
	egl_bind.context = android_bind->context;
	createInfo->next = &egl_bind;
	xrCreateSession(instance, createInfo, session);
}



XrResult bionic_xrGetInstanceProcAddr(XrInstance instance, const char *name, PFN_xrVoidFunction *func)
{
	if(!strcmp(name, "xrInitializeLoaderKHR")) {
		*func = xrInitializeLoaderKHR_noop;
		return (XrResult)xrInitializeLoaderKHR_noop; //TODO: is this correct return value?
	} else {
		return xrGetInstanceProcAddr(instance, name, func);
	}
}

void * bionic_xrCreateInstance(XrInstanceCreateInfo *createInfo, XrInstance *instance)
{
	const char* enabled_exts[2] = {"XR_KHR_opengl_es_enable", "XR_MNDX_egl_enable"};
	createInfo->enabledExtensionCount = 2;
	createInfo->enabledExtensionNames = enabled_exts;
	xrCreateInstance(createInfo, instance);
}
