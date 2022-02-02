package android.opengl;

import android.util.AttributeSet;
import android.content.Context;

import javax.microedition.khronos.egl.EGLContext;
import javax.microedition.khronos.egl.EGL10;
import javax.microedition.khronos.egl.EGLDisplay;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import com.google.android.gles_jni.EGLImpl;

import android.view.View;

import android.view.MotionEvent;

public class GLSurfaceView extends View { // TODO: have this extend SurfaceView once that one is implemented?
	EGLContextFactory context_factory = null;
	EGLConfigChooser config_chooser = null;
	EGL10 java_egl_wrapper;

	public GLSurfaceView(AttributeSet attrs) {
		super(attrs);

		java_egl_wrapper = new EGLImpl();
		native_constructor(attrs);
	}

	public GLSurfaceView(Context context) {
		super(context);

		java_egl_wrapper = new EGLImpl();
		native_constructor(context);
	}

	private native void native_constructor(AttributeSet attrs);
	private native void native_constructor(Context context);

	public boolean onTouchEvent (MotionEvent event) {
		return true;
	}

	public void setEGLConfigChooser(EGLConfigChooser chooser) {
		config_chooser = chooser;
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
		System.out.println("setEGLContextClientVersion("+version+") called");
//		checkRenderThreadState();
//		mEGLContextClientVersion = version;
	}

	private native void native_set_renderer(Renderer renderer, boolean implements_onTouchEvent);

	public void setRenderer(Renderer renderer) {
		System.out.println("setRenderer("+renderer+") called");

		boolean implements_onTouchEvent;

		try {
			Class[] cArg = new Class[1];
			cArg[0] = MotionEvent.class;
			implements_onTouchEvent = !( this.getClass().getMethod("onTouchEvent", cArg).getDeclaringClass() == View.class );
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
		if(config_chooser != null) {
			EGLConfig egl_config = config_chooser.chooseConfig(java_egl_wrapper, new EGLDisplay(egl_display));
			return egl_config.native_egl_config;
		}
		else {
			throw new java.lang.NullPointerException("FIXME: EGLConfigChooser is NULL (time to provide a default implementation?)");
		}
	}

	private long wrap_EGLContextFactory_createContext(long egl_display, long egl_config) {
		if(context_factory != null) {
			EGLContext egl_context = context_factory.createContext(java_egl_wrapper, new EGLDisplay(egl_display), new EGLConfig(egl_config));
			return egl_context.native_egl_context;
		}
		else {
			throw new java.lang.NullPointerException("FIXME: EGLContextFactory is NULL (time to provide a default implementation?)");
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
