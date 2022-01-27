package android.opengl;

import android.util.AttributeSet;
import android.content.Context;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.view.View;

import android.view.MotionEvent;

public class GLSurfaceView extends View { // TODO: have this extend SurfaceView once that one is implemented?
	public GLSurfaceView(AttributeSet attrs) {
		super(attrs);

		native_constructor(attrs);
	}

	public GLSurfaceView(Context context) {
		super(context);

		native_constructor(context);
	}

	private native void native_constructor(AttributeSet attrs);
	private native void native_constructor(Context context);

	public boolean onTouchEvent (MotionEvent event) {
		return true;
	}

	public void setEGLConfigChooser(EGLConfigChooser configChooser) {
//		checkRenderThreadState();
//		mEGLConfigChooser = configChooser;
	}

	public void setEGLConfigChooser(int redSize, int greenSize, int blueSize,
			int alphaSize, int depthSize, int stencilSize) {
//		setEGLConfigChooser(new ComponentSizeChooser(redSize, greenSize,
//				blueSize, alphaSize, depthSize, stencilSize));
	}

    public void setEGLContextFactory(EGLContextFactory factory) {
//        checkRenderThreadState();
//        mEGLContextFactory = factory;
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
		/**
		 * Called when the surface is created or recreated.
		 * <p>
		 * Called when the rendering thread
		 * starts and whenever the EGL context is lost. The EGL context will typically
		 * be lost when the Android device awakes after going to sleep.
		 * <p>
		 * Since this method is called at the beginning of rendering, as well as
		 * every time the EGL context is lost, this method is a convenient place to put
		 * code to create resources that need to be created when the rendering
		 * starts, and that need to be recreated when the EGL context is lost.
		 * Textures are an example of a resource that you might want to create
		 * here.
		 * <p>
		 * Note that when the EGL context is lost, all OpenGL resources associated
		 * with that context will be automatically deleted. You do not need to call
		 * the corresponding "glDelete" methods such as glDeleteTextures to
		 * manually delete these lost resources.
		 * <p>
		 * @param gl the GL interface. Use <code>instanceof</code> to
		 * test if the interface supports GL11 or higher interfaces.
		 * @param config the EGLConfig of the created surface. Can be used
		 * to create matching pbuffers.
		 */
		void onSurfaceCreated(GL10 gl, EGLConfig config);

		/**
		 * Called when the surface changed size.
		 * <p>
		 * Called after the surface is created and whenever
		 * the OpenGL ES surface size changes.
		 * <p>
		 * Typically you will set your viewport here. If your camera
		 * is fixed then you could also set your projection matrix here:
		 * <pre class="prettyprint">
		 * void onSurfaceChanged(GL10 gl, int width, int height) {
		 *	 gl.glViewport(0, 0, width, height);
		 *	 // for a fixed camera, set the projection too
		 *	 float ratio = (float) width / height;
		 *	 gl.glMatrixMode(GL10.GL_PROJECTION);
		 *	 gl.glLoadIdentity();
		 *	 gl.glFrustumf(-ratio, ratio, -1, 1, 1, 10);
		 * }
		 * </pre>
		 * @param gl the GL interface. Use <code>instanceof</code> to
		 * test if the interface supports GL11 or higher interfaces.
		 * @param width
		 * @param height
		 */
		void onSurfaceChanged(GL10 gl, int width, int height);

		/**
		 * Called to draw the current frame.
		 * <p>
		 * This method is responsible for drawing the current frame.
		 * <p>
		 * The implementation of this method typically looks like this:
		 * <pre class="prettyprint">
		 * void onDrawFrame(GL10 gl) {
		 *	 gl.glClear(GL10.GL_COLOR_BUFFER_BIT | GL10.GL_DEPTH_BUFFER_BIT);
		 *	 //... other gl calls to render the scene ...
		 * }
		 * </pre>
		 * @param gl the GL interface. Use <code>instanceof</code> to
		 * test if the interface supports GL11 or higher interfaces.
		 */
		void onDrawFrame(GL10 gl);
	}

	public interface EGLConfigChooser {
		/**
		 * Choose a configuration from the list. Implementors typically
		 * implement this method by calling
		 * {@link EGL10#eglChooseConfig} and iterating through the results. Please consult the
		 * EGL specification available from The Khronos Group to learn how to call eglChooseConfig.
		 * @param egl the EGL10 for the current display.
		 * @param display the current display.
		 * @return the chosen configuration.
		 */
//		EGLConfig chooseConfig(EGL10 egl, EGLDisplay display);
	}

    public interface EGLContextFactory {
//        EGLContext createContext(EGL10 egl, EGLDisplay display, EGLConfig eglConfig);
//        void destroyContext(EGL10 egl, EGLDisplay display, EGLContext context);
    }
}
