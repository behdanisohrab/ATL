package android.app;

import android.R;
import android.content.BroadcastReceiver;
import android.content.ComponentName;
import android.content.Context;
import android.content.ContextWrapper;
import android.content.Intent;
import android.content.res.Configuration;
import android.content.res.XmlResourceParser;
import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.View;
import android.view.ViewGroup;
import android.view.Window;
import android.view.WindowManager;
import android.view.WindowManagerImpl;
import com.reandroid.arsc.chunk.xml.AndroidManifestBlock;
import java.io.IOException;
import java.io.InputStream;
import java.lang.reflect.Constructor;
import java.lang.reflect.InvocationTargetException;
import java.util.ArrayList;
import java.util.List;

public class Activity extends ContextWrapper implements Window.Callback {
	LayoutInflater layout_inflater;
	Window window = new Window(this);
	int requested_orientation = -1 /*ActivityInfo.SCREEN_ORIENTATION_UNSPECIFIED*/; // dummy
	public Intent intent;
	private Activity resultActivity;
	private int resultRequestCode;
	private int pendingRequestCode;
	private int pendingResultCode;
	private Intent pendingData;
	private boolean paused = false;
	List<Fragment> fragments = new ArrayList<>();

	/**
	 * Helper function to be called from native code to construct main activity
	 *
	 * @param className  class name of activity or null
	 * @return  instance of main activity class
	 */
	private static Activity createMainActivity(String className, long native_window) throws ClassNotFoundException, NoSuchMethodException, SecurityException, InstantiationException, IllegalAccessException, IllegalArgumentException, InvocationTargetException, IOException {
		if (className == null) {
			InputStream inStream = ClassLoader.getSystemClassLoader().getResourceAsStream("AndroidManifest.xml");
			AndroidManifestBlock block = AndroidManifestBlock.load(inStream);
			className = block.getMainActivity().searchAttributeByResourceId(AndroidManifestBlock.ID_name).getValueAsString();
			if (className.startsWith(".")) {
				className = block.getPackageName() + className;
			}
		} else {
			className = className.replace('/', '.');
		}
		Class<? extends Activity> cls = Class.forName(className).asSubclass(Activity.class);
		Constructor<? extends Activity> constructor = cls.getConstructor();
		Activity activity = constructor.newInstance();
		activity.window.native_window = native_window;
		return activity;
	}

	public Activity() {
		super(null);
		layout_inflater = new LayoutInflater();
		intent = new Intent();
	}

	public View root_view;

	public final Application getApplication() {
		return (Application)getApplicationContext();
	}

	public WindowManager getWindowManager() {
		return new WindowManagerImpl();
	}

	public String getCallingPackage() {
		return null; // [from api reference] Note: if the calling activity is not expecting a result (that is it did not use the startActivityForResult(Intent, int) form that includes a request code), then the calling package will be null.
	}

	public ComponentName getComponentName() {
		return null;
	}

	public Intent getIntent() {
		return intent;
	}

	public int getRequestedOrientation() {
		return requested_orientation;
	}

	public void setRequestedOrientation(int orientation) {
		requested_orientation = orientation;
	}

	public boolean isFinishing() {
		return false; // FIXME
	}

	public final boolean requestWindowFeature(int featureId) {
		return false; // whatever feature it is, it's probably not supported
	}

	public final void setVolumeControlStream(int streamType) {}

	protected void onCreate(Bundle savedInstanceState) {
		System.out.println("- onCreate - yay!");
		new ViewGroup(this).setId(R.id.content);

		for (Fragment fragment : fragments) {
			fragment.onCreate(savedInstanceState);
		}

		return;
	}

	protected void onStart() {
		System.out.println("- onStart - yay!");
		if (window.contentView != null)
			window.setContentView(window.contentView);

		for (Fragment fragment : fragments) {
			fragment.onStart();
		}

		return;
	}

	protected void onRestart() {
		System.out.println("- onRestart - yay!");

		return;
	}

	protected void onResume() {
		System.out.println("- onResume - yay!");
		if (pendingData != null) {
			onActivityResult(pendingRequestCode, pendingResultCode, pendingData);
			pendingData = null;
		}

		for (Fragment fragment : fragments) {
			fragment.onResume();
		}

		paused = false;
		return;
	}

	protected void onPause() {
		System.out.println("- onPause - yay!");

		for (Fragment fragment : fragments) {
			fragment.onPause();
		}

		paused = true;
		return;
	}

	protected void onStop() {
		System.out.println("- onStop - yay!");

		for (Fragment fragment : fragments) {
			fragment.onStop();
		}

		return;
	}

	protected void onDestroy() {
		System.out.println("- onDestroy - yay!");

		for (Fragment fragment : fragments) {
			fragment.onDestroy();
		}

		return;
	}

	public void onWindowFocusChanged(boolean hasFocus) {
		System.out.println("- onWindowFocusChanged - yay! (hasFocus: " + hasFocus + ")");

		return;
	}

	protected void onSaveInstanceState(Bundle outState) {
	}

	void onConfigurationChanged(Configuration newConfig) {
	}

	public void onLowMemory() {
	}

	/* --- */

	public void setContentView(int layoutResID) throws Exception {
		System.out.println("- setContentView - yay!");

		XmlResourceParser xpp = Context.this_application.getResources().getLayout(layoutResID);

		root_view = layout_inflater.inflate(xpp, null, false);

		System.out.println("~~~~~~~~~~~");
		System.out.println(root_view);
		System.out.println(root_view.widget);
		System.out.println("~~~~~~~~~~~");

		setContentView(root_view);

/*		Window w = new Window();
		w.setTitle(this.toString());
		w.setDefaultSize(540, 960);
		w.add(root_view.widget);
		w.showAll();


		w.connect(new Window.DeleteEvent() {
			public boolean onDeleteEvent(Widget source, Event event) {
				Gtk.mainQuit();
				return false;
			}
		});*/
	}

	public void setContentView(View view, ViewGroup.LayoutParams layoutParams) {
		setContentView(view);
	}

	public void setContentView(View view) {
		window.setContentView(view);
	}

	public <T extends android.view.View> T findViewById(int id) {
		System.out.println("- findViewById - asked for view with id: " + id);
		View view = null;
		if (window.contentView != null)
			view = window.contentView.findViewById(id);
		System.out.println("- findViewById - found this: " + view);

		return (T)view;
	}

	public void invalidateOptionsMenu() {
		System.out.println("invalidateOptionsMenu() called, should we do something?");
	}

	public Window getWindow() {
		return this.window;
	}

	public final void runOnUiThread(Runnable action) {
		if(Looper.myLooper() == Looper.getMainLooper()) {
			action.run();
		} else {
			new Handler(Looper.getMainLooper()).post(action);
		}
	}

	protected void onActivityResult(int requestCode, int resultCode, Intent data) {}

	public void startActivityForResult(Intent intent, int requestCode) {
		System.out.println("startActivityForResult(" + intent + ", " + requestCode + ") called, but we don't currently support multiple activities");
		if (intent.getComponent() != null) {
			try {
				Class<? extends Activity> cls = Class.forName(intent.getComponent().getClassName()).asSubclass(Activity.class);
				Constructor<? extends Activity> constructor = cls.getConstructor();
				final Activity activity = constructor.newInstance();
				activity.intent = intent;
				activity.getWindow().native_window = getWindow().native_window;
				activity.resultRequestCode = requestCode;
				activity.resultActivity = this;
				runOnUiThread(new Runnable() {
					@Override
					public void run() {
						nativeStartActivity(activity);
					}
				});
			} catch (ClassNotFoundException | NoSuchMethodException | SecurityException | InstantiationException | IllegalAccessException | IllegalArgumentException | InvocationTargetException e) {
				onActivityResult(requestCode, 0 /*RESULT_CANCELED*/, new Intent()); // RESULT_CANCELED is the only pre-defined return value, so hopefully it works out for us
			}
		}
		else {
			onActivityResult(requestCode, 0 /*RESULT_CANCELED*/, new Intent()); // RESULT_CANCELED is the only pre-defined return value, so hopefully it works out for us
		}
	}

	public void setResult(int resultCode, Intent data) {
		if (resultActivity != null) {
			resultActivity.pendingRequestCode = resultRequestCode;
			resultActivity.pendingResultCode = resultCode;
			resultActivity.pendingData = data;
		}
	}

	public void setResult(int resultCode) {
		setResult(resultCode, null);
	}

	public final void showDialog(int id) {
		System.out.println("Activity.showDialog(" + id + ") called");
	}

	public void finish() {
		runOnUiThread(new Runnable() {
			@Override
			public void run() {
				nativeFinish(getWindow().native_window);
			}
		});
	}

	public Object getLastNonConfigurationInstance() {
		return null;
	}

	public FragmentManager getFragmentManager() {
		return new FragmentManager(this);
	}

	public LayoutInflater getLayoutInflater() {
		return layout_inflater;
	}

	public CharSequence getTitle() {
		return "Title";
	}

	public boolean isChangingConfigurations() {return false;}

	@Override
	public void onContentChanged() {
		// TODO Auto-generated method stub
		throw new UnsupportedOperationException("Unimplemented method 'onContentChanged'");
	}

	public boolean onCreateOptionsMenu(Menu menu) {
		return true;
	}

	@Override
	public boolean onCreatePanelMenu(int featureId, Menu menu) {
		if (featureId == Window.FEATURE_OPTIONS_PANEL) {
			return onCreateOptionsMenu(menu);
		}
		return false;
	}

	@Override
	public View onCreatePanelView(int featureId) {
		return null;
	}

	public MenuInflater getMenuInflater() {
		return new MenuInflater(this);
	}

	public boolean onPrepareOptionsMenu(Menu menu) {
		return true;
	}

	@Override
	public boolean onPreparePanel(int featureId, View view, Menu menu) {
		if (featureId == Window.FEATURE_OPTIONS_PANEL && menu != null) {
			return onPrepareOptionsMenu(menu);
		}
		return true;
	}

	@Override
	public boolean onMenuItemSelected(int featureId, MenuItem item) {
		if (featureId == Window.FEATURE_OPTIONS_PANEL) {
			return onOptionsItemSelected(item);
		}
		return false;
	}

	public boolean onOptionsItemSelected(MenuItem item) {
		return false;
	}

	public void onOptionsMenuClosed(Menu menu) {}

	@Override
	public void onPanelClosed(int featureId, Menu menu) {
		if (featureId == Window.FEATURE_OPTIONS_PANEL) {
			onOptionsMenuClosed(menu);
		}
	}

	public void setTitle(CharSequence title) {}

	public void onBackPressed() {
		finish();
	}

	public void setIntent(Intent newIntent) {}

	public void unregisterReceiver(BroadcastReceiver receiver) {}

	public Intent getParentActivityIntent() {
		return null;
	}

	@Override
	public boolean onMenuOpened(int featureId, Menu menu) {
		System.out.println("onMenuOpened(" + featureId + ", " + menu + ") called");
		return false;
	}

	public void recreate() {
		try {
			/* TODO: check if this is a toplevel activity */
			Class<? extends Activity> cls = this.getClass();
			Constructor<? extends Activity> constructor = cls.getConstructor();
			Activity activity = constructor.newInstance();
			activity.getWindow().native_window = getWindow().native_window;
			System.out.println("activity.getWindow().native_window >"+activity.getWindow().native_window+"<");
			nativeFinish(0);
			nativeRecreateActivity(activity);
		} catch (NoSuchMethodException | SecurityException | InstantiationException | IllegalAccessException | IllegalArgumentException | InvocationTargetException e) {
			System.out.println("exception in Activity.recreate, this is kinda sus");
			e.printStackTrace();
		}
	}

	private native void nativeFinish(long native_window);
	public static native void nativeRecreateActivity(Activity activity);
	public static native void nativeStartActivity(Activity activity);
	public static native void nativeOpenURI(String uri);
	public static native void nativeShare(String text);
}
