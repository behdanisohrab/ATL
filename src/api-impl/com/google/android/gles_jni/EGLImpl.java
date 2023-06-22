package com.google.android.gles_jni;

import javax.microedition.khronos.egl.*;

public class EGLImpl implements EGL10 {
	private native long native_eglCreateContext(long egl_display, long egl_config, EGLContext share_context, int[] attrib_list);

	public EGLContext eglCreateContext(EGLDisplay display, EGLConfig config, EGLContext share_context, int[] attrib_list) {
		long native_egl_context = native_eglCreateContext(display.native_egl_display, config.native_egl_config, share_context, attrib_list);
		if (native_egl_context == 0) {
			return EGL10.EGL_NO_CONTEXT;
		}
		return new EGLContext(native_egl_context);
	}

	private native boolean native_eglChooseConfig(long egl_display, int[] attrib_list, long[] egl_configs, int config_size, int[] num_config);

	public boolean eglChooseConfig(EGLDisplay display, int[] attrib_list, EGLConfig[] configs, int config_size, int[] num_config) {
		long[] native_egl_configs = null;

		if (config_size != 0) {
			if (configs == null) {
				throw new java.lang.IllegalArgumentException("app error: eglChooseConfig called with non-zero 'config_size' (" + config_size + "), but with null 'configs' array");
			}
			native_egl_configs = new long[config_size];
		}

		boolean ret = native_eglChooseConfig(display.native_egl_display, attrib_list, native_egl_configs, config_size, num_config);

		if (configs != null) {
			for (int i = 0; i < configs.length; i++) {
				configs[i] = new EGLConfig(native_egl_configs[i]);
			}
		}

		return ret;
	}

	public boolean eglCopyBuffers(EGLDisplay display, EGLSurface surface, Object native_pixmap) { return false; }
	public EGLSurface eglCreatePbufferSurface(EGLDisplay display, EGLConfig config, int[] attrib_list) { return null; }
	public EGLSurface eglCreatePixmapSurface(EGLDisplay display, EGLConfig config, Object native_pixmap, int[] attrib_list) { return null; }
	public EGLSurface eglCreateWindowSurface(EGLDisplay display, EGLConfig config, Object native_window, int[] attrib_list) { return null; }
	public boolean eglDestroyContext(EGLDisplay display, EGLContext context) { return false; }
	public boolean eglDestroySurface(EGLDisplay display, EGLSurface surface) { return false; }
	public boolean eglGetConfigAttrib(EGLDisplay display, EGLConfig config, int attribute, int[] value) { return false; }
	public boolean eglGetConfigs(EGLDisplay display, EGLConfig[] configs, int config_size, int[] num_config) { return false; }
	public EGLContext eglGetCurrentContext() { return null; }
	public EGLDisplay eglGetCurrentDisplay() { return null; }
	public EGLSurface eglGetCurrentSurface(int readdraw) { return null; }
	public EGLDisplay eglGetDisplay(Object native_display) { return null; }
	public int eglGetError() { return EGL_SUCCESS; } // don't let yourself get fooled, this is also a stub :P
	public boolean eglInitialize(EGLDisplay display, int[] major_minor) { return false; }
	public boolean eglMakeCurrent(EGLDisplay display, EGLSurface draw, EGLSurface read, EGLContext context) { return false; }
	public boolean eglQueryContext(EGLDisplay display, EGLContext context, int attribute, int[] value) { return false; }
	public String eglQueryString(EGLDisplay display, int name) { return "FIXME"; }
	public boolean eglQuerySurface(EGLDisplay display, EGLSurface surface, int attribute, int[] value) { return false; }
	/**
	 * @hide *
	 */
	public boolean eglReleaseThread() { return false; }
	public boolean eglSwapBuffers(EGLDisplay display, EGLSurface surface) { return false; }
	public boolean eglTerminate(EGLDisplay display) { return false; }
	public boolean eglWaitGL() { return false; }
	public boolean eglWaitNative(int engine, Object bindTarget) { return false; }
}
