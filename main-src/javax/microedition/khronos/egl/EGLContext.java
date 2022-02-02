package javax.microedition.khronos.egl;

import javax.microedition.khronos.opengles.GL;

public class EGLContext
{
	private static final EGL EGL_INSTANCE = null;//new com.google.android.gles_jni.EGLImpl();
	public long native_egl_context = 0;

	public static EGL getEGL() {
		return EGL_INSTANCE;
	}

	public EGLContext(long native_egl_context) {
		this.native_egl_context = native_egl_context;
	}

	public GL getGL() {
		return null;
	}
}
