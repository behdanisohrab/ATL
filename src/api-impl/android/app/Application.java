package android.app;

import android.os.Bundle;
import android.content.res.Configuration;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import com.reandroid.apk.AndroidFrameworks;
import com.reandroid.arsc.chunk.xml.AndroidManifestBlock;
import java.io.InputStream;
import java.io.IOException;

import android.content.ContextWrapper;

public class Application extends ContextWrapper {
	private String app_icon_path = null;

	private String get_app_icon_path() {
		return app_icon_path;
	}

	public interface ActivityLifecycleCallbacks {
		void onActivityCreated(Activity activity, Bundle savedInstanceState);
		void onActivityStarted(Activity activity);
		void onActivityResumed(Activity activity);
		void onActivityPaused(Activity activity);
		void onActivityStopped(Activity activity);
		void onActivitySaveInstanceState(Activity activity, Bundle outState);
		void onActivityDestroyed(Activity activity);
	}
	/**
	 * Callback interface for use with {@link Application#registerOnProvideAssistDataListener}
	 * and {@link Application#unregisterOnProvideAssistDataListener}.
	 */
	public interface OnProvideAssistDataListener {
		/**
		 * This is called when the user is requesting an assist, to build a full
		 * {@link Intent#ACTION_ASSIST} Intent with all of the context of the current
		 * application.  You can override this method to place into the bundle anything
		 * you would like to appear in the {@link Intent#EXTRA_ASSIST_CONTEXT} part
		 * of the assist Intent.
		 */
		public void onProvideAssistData(Activity activity, Bundle data);
	}

	public Application() {
		super(null);
		/* TODO: is this the right place to put this? */
		InputStream inStream = ClassLoader.getSystemClassLoader().getResourceAsStream("AndroidManifest.xml");
		try {
			AndroidManifestBlock manifest = AndroidManifestBlock.load(inStream);
			int app_icon_resid = manifest.getIconResourceId();
			app_icon_path = this.getResources().getString(app_icon_resid);
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
	/**
	 * Called when the application is starting, before any activity, service,
	 * or receiver objects (excluding content providers) have been created.
	 * Implementations should be as quick as possible (for example using
	 * lazy initialization of state) since the time spent in this function
	 * directly impacts the performance of starting the first activity,
	 * service, or receiver in a process.
	 * If you override this method, be sure to call super.onCreate().
	 */
	public void onCreate() {
	}
	/**
	 * This method is for use in emulated process environments.  It will
	 * never be called on a production Android device, where processes are
	 * removed by simply killing them; no user code (including this callback)
	 * is executed when doing so.
	 */
	public void onTerminate() {
	}
	public void onConfigurationChanged(Configuration newConfig) {
	}
	public void onLowMemory() {
	}
	public void onTrimMemory(int level) {
	}
/*	public void registerComponentCallbacks(ComponentCallbacks callback) {
	}
	public void unregisterComponentCallbacks(ComponentCallbacks callback) {
	}*/
	public void registerActivityLifecycleCallbacks(ActivityLifecycleCallbacks callback) {
	}
	public void unregisterActivityLifecycleCallbacks(ActivityLifecycleCallbacks callback) {
	}
	public void registerOnProvideAssistDataListener(OnProvideAssistDataListener callback) {
	}
	public void unregisterOnProvideAssistDataListener(OnProvideAssistDataListener callback) {
	}
}
