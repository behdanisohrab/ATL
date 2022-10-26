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

//#include <gtk/gtk.h>
#include <gdk/wayland/gdkwayland.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#define GLFW_EXPOSE_NATIVE_WAYLAND
#include <GLFW/glfw3native.h>

#include <jni.h>


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
	GLFWwindow *glfw_window;
	EGLNativeWindowType egl_window;
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
   int v = 0;
   glfwGetWindowSize(native_window->glfw_window, &v, NULL);
   return v;
}

int32_t ANativeWindow_getHeight(struct ANativeWindow *native_window)
{
   int v = 0;
   glfwGetWindowSize(native_window->glfw_window, NULL, &v);
   return v;
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

// FIXME: include the header
struct wl_surface;
struct wl_egl_window *wl_egl_window_create(struct wl_surface *surface,int width, int height);

static void
glfw_error_cb(int code, const char *error)
{
   fprintf(stderr, "glfw: (%d) %s\n", code, error);
}

ANativeWindow * ANativeWindow_fromSurface(JNIEnv* env, jobject surface)
{
	// FIXME: add a path for x11
// TODO: something with subsurfaces
/*	GdkDisplay *display = gdk_display_get_default(); //TODO: edge cases?
	struct wl_display *wl_display = gdk_wayland_display_get_wl_display(display);
	struct wl_compositor *wl_compositor = gdk_wayland_display_get_wl_compositor(display);
	//GdkSurface *popup_surface = gdk_surface_new_popup (GdkSurface* parent, gboolean autohide) // TODO: could this work better for us? (need to somehow get the parent surface)
	struct wl_surface *toplevel_surface = gdk_wayland_surface_get_wl_surface(gdk_surface_new_toplevel(display));
	struct wl_surface *our_surface = wl_compositor_create_surface(wl_compositor);
//	wl_subsurface* our_subsurface = wl_subcompositor_get_subsurface(wl_compositor, our_surface; our_parent);
	struct wl_egl_window *egl_window = wl_egl_window_create(our_surface, 700, 700);
	return (ANativeWindow *)egl_window;*/

	struct ANativeWindow *native_window = malloc(sizeof(struct ANativeWindow));

	glfwInit();
	fprintf(stderr, "glfw: %s\n", glfwGetVersionString());
	glfwSetErrorCallback(glfw_error_cb);
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	native_window->glfw_window = glfwCreateWindow(700, 700, "FIXME: don't create a separate window for this widget", NULL, NULL);
	struct wl_surface *wayland_surface = glfwGetWaylandWindow(native_window->glfw_window);
	int width, height;
	glfwGetWindowSize(native_window->glfw_window, &width, &height);
	printf("glfw::: width: %d, height: %d\n", width, height);
	native_window->egl_window = (EGLNativeWindowType)wl_egl_window_create(wayland_surface, width, height);

	printf("EGL::: wayland_surface: %p\n", wayland_surface);
	printf("EGL::: native_window->glfw_window: %p\n", native_window->glfw_window);
	printf("EGL::: native_window->egl_window: %ld\n", native_window->egl_window);

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

EGLDisplay bionic_eglGetDisplay(NativeDisplayType native_display)
{
	/*
	 * On android, at least SDL passes 0 (EGL_DISPLAY_DEFAULT) to eglGetDisplay and uses the resulting display.
	 * We obviously want to make the app use the correct display, which may happen to be a different one
	 * than the "default" display (especially on Wayland)
	 */
	glfwInit(); // it's allegedly safe to call this multiple times
	struct wl_display *glfw_wl_display = glfwGetWaylandDisplay(); 
	return eglGetPlatformDisplay(EGL_PLATFORM_WAYLAND_KHR, glfw_wl_display, NULL);
}

EGLSurface bionic_eglCreateWindowSurface(EGLDisplay display, EGLConfig config, struct ANativeWindow *native_window, EGLint const *attrib_list)
{
	// TODO: eglGetDisplay((EGLNativeDisplayType)0) isn't ideal...
	PrintConfigAttributes(display, config);
	EGLSurface ret = eglCreateWindowSurface(display, config, native_window->egl_window, attrib_list);

	printf("EGL::: native_window->glfw_window: %p\n", native_window->glfw_window);
	printf("EGL::: native_window->egl_window: %ld\n", native_window->egl_window);
	printf("EGL::: eglGetError: %d\n", eglGetError());

	printf("EGL::: ret: %p\n", ret);

	return ret;
}
