package android.opengl;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Rect;
import android.util.AttributeSet;
import android.view.MotionEvent;
import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.View;
import com.google.android.gles_jni.EGLImpl;
import javax.microedition.khronos.egl.EGL10;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.egl.EGLContext;
import javax.microedition.khronos.egl.EGLDisplay;
import javax.microedition.khronos.opengles.GL10;

public class GLSurfaceView extends View implements SurfaceHolder.Callback { // TODO: have this extend SurfaceView?
	EGLContextFactory context_factory = new default_ContextFactory();
	EGLConfigChooser config_chooser = new boolean_ConfigChooser(true);
	EGL10 java_egl_wrapper = (EGL10)EGLContext.getEGL();
	GL10 java_gl_wrapper = (GL10)EGLContext.getGL();
	int opengl_version = 1;

	// from SurfaceHolder.Callback
	public void surfaceCreated(SurfaceHolder surfaceHolder) {}
	public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {}
	public void surfaceDestroyed(SurfaceHolder holder) {}

	public GLSurfaceView(Context context, AttributeSet attrs) {
		super(context, attrs);
	}

	public GLSurfaceView(Context context) {
		super(context);
	}

	@Override
	protected native long native_constructor(Context context, AttributeSet attrs);

	// Resumes the rendering thread, re-creating the OpenGL context if necessary. It is the counterpart to onPause(). This method should typically be called in Activity.onStart. Must not be called before a renderer has been set.
	// TODO: make use of this
	public void onResume() {}

	public boolean onTouchEvent(MotionEvent event) {
		return true;
	}

	public void setEGLConfigChooser(EGLConfigChooser chooser) {
		config_chooser = chooser;
	}

	public void setEGLConfigChooser(boolean needDepth) {
		config_chooser = new boolean_ConfigChooser(needDepth);
	}

	public void setEGLConfigChooser(int redSize, int greenSize, int blueSize,
					int alphaSize, int depthSize, int stencilSize) {
		//		setEGLConfigChooser(new ComponentSizeChooser(redSize, greenSize,
		//				blueSize, alphaSize, depthSize, stencilSize));
	}

	public void setEGLContextFactory(EGLContextFactory factory) {
		context_factory = factory;
	}

	public void setEGLContextClientVersion(int version) {
		opengl_version = version;
	}

	public void setPreserveEGLContextOnPause(boolean preserveOnPause) {}

	public synchronized boolean shouldTerminateEGLWhenPausing() {
		return false;
	}

	private native void native_set_renderer(Renderer renderer, boolean implements_onTouchEvent);

	public void setRenderer(Renderer renderer) {
		System.out.println("setRenderer(" + renderer + ") called");

		boolean implements_onTouchEvent;

		try {
			Class[] cArg = new Class[1];
			cArg[0] = MotionEvent.class;
			implements_onTouchEvent = !(this.getClass().getMethod("onTouchEvent", cArg).getDeclaringClass() == View.class);
		} catch (NoSuchMethodException e) {
			implements_onTouchEvent = false;
		}

		native_set_renderer(renderer, implements_onTouchEvent);
/*		checkRenderThreadState();
		if (mEGLConfigChooser == null) {
			mEGLConfigChooser = new SimpleEGLConfigChooser(true);
		}
		if (mEGLContextFactory == null) {
			mEGLContextFactory = new DefaultContextFactory();
		}
		if (mEGLWindowSurfaceFactory == null) {
			mEGLWindowSurfaceFactory = new DefaultWindowSurfaceFactory();
		}
		mRenderer = renderer;
		mGLThread = new GLThread(mThisWeakRef);
		mGLThread.start();*/
	}

	public interface Renderer {
		void onSurfaceCreated(GL10 gl, EGLConfig config);
		void onSurfaceChanged(GL10 gl, int width, int height);
		void onDrawFrame(GL10 gl);
	}

	private long wrap_EGLConfigChooser_chooseConfig(long egl_display) {
		if (config_chooser != null) {
			EGLConfig egl_config = config_chooser.chooseConfig(java_egl_wrapper, new EGLDisplay(egl_display));
			return egl_config.native_egl_config;
		} else {
			throw new java.lang.NullPointerException("FIXME: EGLConfigChooser is NULL (time to provide a default implementation?)");
		}
	}

	private long wrap_EGLContextFactory_createContext(long egl_display, long egl_config) {
		if (context_factory != null) {
			EGLContext egl_context = context_factory.createContext(java_egl_wrapper, new EGLDisplay(egl_display), new EGLConfig(egl_config));
			return egl_context.native_egl_context;
		} else {
			throw new java.lang.NullPointerException("FIXME: EGLContextFactory is NULL (time to provide a default implementation?)");
		}
	}

	public void queueEvent(Runnable r) {
		// just run it synchronously, why not
		r.run();
	}

	private class DummySurfaceHolder implements SurfaceHolder {
		public void addCallback(Callback callback) {}
		public void removeCallback(Callback callback) {}
		public boolean isCreating() { return false; }
		public void setType(int type) {}
		public void setFixedSize(int width, int height) {}
		public void setSizeFromLayout() {}
		public void setFormat(int format) {}
		public void setKeepScreenOn(boolean screenOn) {}
		public Canvas lockCanvas() { return null; }
		public Canvas lockCanvas(Rect dirty) { return null; }
		public void unlockCanvasAndPost(Canvas canvas) {}
		public Rect getSurfaceFrame() { return null; }
		public Surface getSurface() { return null; }
	}

	public SurfaceHolder getHolder() {
		return (SurfaceHolder) new DummySurfaceHolder();
	}

	private static class boolean_ConfigChooser implements GLSurfaceView.EGLConfigChooser {
		// TODO - what happens if we actually allow for 16 bits per color?
		private static int[] config_attribs_no_depth = {EGL10.EGL_RED_SIZE, 8, EGL10.EGL_GREEN_SIZE, 8, EGL10.EGL_BLUE_SIZE, 8, EGL10.EGL_ALPHA_SIZE, 0, EGL10.EGL_DEPTH_SIZE, 0, EGL10.EGL_STENCIL_SIZE, 0, EGL10.EGL_NONE};
		private static int[] config_attribs_depth = {EGL10.EGL_RED_SIZE, 8, EGL10.EGL_GREEN_SIZE, 8, EGL10.EGL_BLUE_SIZE, 8, EGL10.EGL_ALPHA_SIZE, 0, EGL10.EGL_DEPTH_SIZE, 16, EGL10.EGL_STENCIL_SIZE, 0, EGL10.EGL_NONE};

		private boolean want_depth;

		public boolean_ConfigChooser(boolean depth) {
			want_depth = depth;
		}

		public EGLConfig chooseConfig(EGL10 egl, EGLDisplay display) {
			int[] num_config = new int[1];
			egl.eglChooseConfig(display, want_depth ? config_attribs_depth : config_attribs_no_depth, null, 0, num_config);
			int numConfigs = num_config[0];
			if (numConfigs <= 0) {
				throw new IllegalArgumentException("boolean_ConfigChooser: no configs match");
			}
			EGLConfig[] configs = new EGLConfig[numConfigs];
			egl.eglChooseConfig(display, want_depth ? config_attribs_depth : config_attribs_no_depth, configs, numConfigs, num_config);
			return configs[0]; // TODO - something smarter here?
		}
	}

	private static class default_ContextFactory implements GLSurfaceView.EGLContextFactory {
		public EGLContext createContext(EGL10 egl, EGLDisplay display, EGLConfig eglConfig) {
			return egl.eglCreateContext(display, eglConfig, EGL10.EGL_NO_CONTEXT, new int[] {EGL10.EGL_CONTEXT_CLIENT_VERSION, 2 /*opengl_version*/, EGL10.EGL_NONE});
		}

		public void destroyContext(EGL10 egl, EGLDisplay display, EGLContext context) {
			egl.eglDestroyContext(display, context);
		}
	}

	// interfaces

	public interface EGLConfigChooser {
		EGLConfig chooseConfig(EGL10 egl, EGLDisplay display);
	}

	public interface EGLContextFactory {
		EGLContext createContext(EGL10 egl, EGLDisplay display, EGLConfig eglConfig);
		//		void destroyContext(EGL10 egl, EGLDisplay display, EGLContext context);
	}
}
