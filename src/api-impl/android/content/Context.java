package android.content;

import android.app.Activity;
import android.app.ActivityManager;
import android.app.AlarmManager;
import android.app.Application;
import android.app.KeyguardManager;
import android.app.NotificationManager;
import android.app.SharedPreferencesImpl;
import android.app.UiModeManager;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageManager;
import android.content.res.AssetManager;
import android.content.res.Configuration;
import android.content.res.Resources;
import android.content.res.TypedArray;
import android.graphics.drawable.Drawable;
import android.hardware.input.InputManager;
import android.hardware.SensorManager;
import android.hardware.display.DisplayManager;
import android.hardware.usb.UsbManager;
import android.location.LocationManager;
import android.media.AudioManager;
import android.media.MediaRouter;
import android.net.ConnectivityManager;
import android.os.Looper;
import android.os.PowerManager;
import android.os.Vibrator;
import android.telephony.TelephonyManager;
import android.util.AttributeSet;
import android.util.DisplayMetrics;
import android.util.Slog;
import android.view.WindowManagerImpl;
import android.view.accessibility.AccessibilityManager;
import android.view.inputmethod.InputMethodManager;

import com.reandroid.arsc.chunk.xml.AndroidManifestBlock;
import com.reandroid.arsc.chunk.xml.ResXmlAttribute;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.InputStream;
import java.lang.reflect.Constructor;
import java.lang.reflect.InvocationTargetException;
import java.io.IOException;

public class Context extends Object {
	private final static String TAG = "Context";

	public static final int MODE_PRIVATE = 0;
	public static final String LOCATION_SERVICE = "location";
	public static final String AUDIO_SERVICE = "audio";
	public static final String DISPLAY_SERVICE = "display";
	public static final String MEDIA_ROUTER_SERVICE = "media_router";
	public static final String WINDOW_SERVICE = "window";
	public static AndroidManifestBlock manifest = null;

	static AssetManager assets;
	static DisplayMetrics dm;
	static Configuration config;
	static Resources r;
	static ApplicationInfo application_info;
	static Resources.Theme theme;

	static String apk_path = "/tmp/APK_PATH_SHOULD_HAVE_BEEN_FILLED_IN_BY_CODE_IN_main.c/";

	public /*← FIXME?*/ static Application this_application;

	File data_dir = null;
	File prefs_dir = null;
	File files_dir = null;
	File obb_dir = null;
	File cache_dir = null;

	static {
		assets = new AssetManager();
		dm = new DisplayMetrics();
		config = new Configuration();
		r = new Resources(assets, dm, config);
		theme = r.newTheme();
		application_info = new ApplicationInfo();
		InputStream inStream = ClassLoader.getSystemClassLoader().getResourceAsStream("AndroidManifest.xml");
		try {
			manifest = AndroidManifestBlock.load(inStream);
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	static Application createApplication(long native_window) throws InstantiationException, IllegalAccessException, IllegalArgumentException, InvocationTargetException, NoSuchMethodException, SecurityException, ClassNotFoundException {
		Application application;
		ResXmlAttribute application_name = manifest.getApplicationElement().searchAttributeByResourceId(AndroidManifestBlock.ID_name);
		String className = (application_name != null) ? application_name.getValueAsString() : "android.app.Application";
		if (className.startsWith(".")) {
			className = manifest.getPackageName() + className;
		}
		Class<? extends Application> cls = Class.forName(className).asSubclass(Application.class);
		Constructor<? extends Application> constructor = cls.getConstructor();
		application = constructor.newInstance();
		ResXmlAttribute application_theme = manifest.getApplicationElement().searchAttributeByResourceId(AndroidManifestBlock.ID_theme);
		if (application_theme != null)
			application.setTheme(application_theme.getData());
		application.native_window = native_window;
		this_application = application;
		return application;
	}

	public Context() {
		Slog.v(TAG, "new Context! this one is: " + this);
	}

	public int checkPermission(String permission, int pid, int uid) {
		return getPackageManager().checkPermission(permission, "dummy");
	}

	public Resources.Theme getTheme() {
		return theme;
	}

	public ApplicationInfo getApplicationInfo() {
		// TODO: do this somewhere saner?
		application_info.nativeLibraryDir = (new File(getDataDirFile(), "lib")).getAbsolutePath();
		return application_info;
	}

	public Context getApplicationContext() {
		return (Context)this_application;
	}

	public ContentResolver getContentResolver() {
		return new ContentResolver();
	}

	public Object getSystemService(String name) {
		switch (name) {
			case "window":
				return new WindowManagerImpl();
			case "clipboard":
				return new ClipboardManager();
			case "sensor":
				return new SensorManager();
			case "connectivity":
				return new ConnectivityManager();
			case "keyguard":
				return new KeyguardManager();
			case "phone":
				return new TelephonyManager();
			case "audio":
				return new AudioManager();
			case "activity":
				return new ActivityManager();
			case "usb":
				return new UsbManager();
			case "vibrator":
				return new Vibrator();
			case "power":
				return new PowerManager();
			case "display":
				return new DisplayManager();
			case "media_router":
				return new MediaRouter();
			case "notification":
				return new NotificationManager();
			case "alarm":
				return new AlarmManager();
			case "input":
				return new InputManager();
			case "location":
				return new LocationManager();
			case "uimode":
				return new UiModeManager();
			case "input_method":
				return new InputMethodManager();
			case "accessibility":
				return new AccessibilityManager();
			default:
				Slog.e(TAG, "!!!!!!! getSystemService: case >" + name + "< is not implemented yet");
				return null;
		}
	}

	public Intent registerReceiver(BroadcastReceiver receiver, IntentFilter filter) {
		return new Intent();
	}

	public Looper getMainLooper() {
		/* TODO: this is not what AOSP does, which could be a problem */
		Looper looper = Looper.myLooper();
		if(looper == null) {
			Looper.prepare();
			looper = Looper.myLooper();
		}

		return looper;
	}

	public String getPackageName() {
		return manifest.getPackageName();
	}

	public String getPackageCodePath() {
		return apk_path;
	}

	public final String getString(int resId) {
		return r.getString(resId);
	}

	public final String getString (int resId, Object... formatArgs) {
		return r.getString(resId, formatArgs);
	}

	public PackageManager getPackageManager() {
		return new PackageManager();
	}

	public Resources getResources() {
		return r;
	}

	public AssetManager getAssets() {
		return assets;
	}

	private File makeFilename(File base, String name) {
		if (name.indexOf(File.separatorChar) < 0) {
			return new File(base, name);
		}
		throw new IllegalArgumentException(
		    "File " + name + " contains a path separator");
	}

	private File getDataDirFile() {
		if (data_dir == null) {
			data_dir = android.os.Environment.getExternalStorageDirectory();
		}
		return data_dir;
	}

	public File getFilesDir() {
		if (files_dir == null) {
			files_dir = new File(getDataDirFile(), "files");
		}
		if (!files_dir.exists()) {
			if (!files_dir.mkdirs()) {
				if (files_dir.exists()) {
					// spurious failure; probably racing with another process for this app
					return files_dir;
				}
				Slog.w(TAG, "Unable to create files directory " + files_dir.getPath());
				return null;
			}
		}
		return files_dir;
	}

	public File getExternalFilesDir(String type) {
		return getFilesDir();
	}

	public File[] getExternalFilesDirs(String type) {
		return new File[] {getExternalFilesDir(type)};
	}

	public File getObbDir() {
		if (obb_dir == null) {
			obb_dir = new File(getDataDirFile(), "Android/obb/" + getPackageName());
		}
		if (!obb_dir.exists()) {
			if (!obb_dir.mkdirs()) {
				if (obb_dir.exists()) {
					// spurious failure; probably racing with another process for this app
					return obb_dir;
				}
				Slog.w(TAG, "Unable to create obb directory >" + obb_dir.getPath() + "<");
				return null;
			}
		}
		return obb_dir;
	}

	public File[] getObbDirs() {
		return new File[] {getObbDir()};
	}

	// FIXME: should be something like /tmp/cache, but may need to create that directory
	public File getCacheDir() {
		if (cache_dir == null) {
			cache_dir = new File("/tmp/");
		}
		return cache_dir;
	}

	public File getExternalCacheDir() {
		return getCacheDir();
	}

	private File getPreferencesDir() {
		if (prefs_dir == null) {
			prefs_dir = new File(getDataDirFile(), "shared_prefs");
		}
		return prefs_dir;
	}

	public File getSharedPrefsFile(String name) {
		return makeFilename(getPreferencesDir(), name + ".xml");
	}

	public SharedPreferences getSharedPreferences(String name, int mode) {
		Slog.v(TAG, "\n\n...> getSharedPreferences(" + name + ")\n\n");
		File prefsFile = getSharedPrefsFile(name);
		return new SharedPreferencesImpl(prefsFile, mode);
	}

	public ClassLoader getClassLoader() {
		// not perfect, but it's what we use for now as well, and it works
		return ClassLoader.getSystemClassLoader();
	}

	public ComponentName startService(Intent service) {
		return new ComponentName("", "");
	}

	// TODO: do these both work? make them look more alike
	public FileInputStream openFileInput(String name) throws FileNotFoundException {
		Slog.v(TAG, "openFileInput called for: '" + name + "'");
		File file = new File(getFilesDir(), name);

		return new FileInputStream(file);
	}

	public FileOutputStream openFileOutput(String name, int mode) throws java.io.FileNotFoundException {
		Slog.v(TAG, "openFileOutput called for: '" + name + "'");
		return new FileOutputStream(android.os.Environment.getExternalStorageDirectory().getPath() + "/files/" + name);
	}

	public int checkCallingOrSelfPermission(String permission) {
		Slog.w(TAG, "!!! app wants to know if it has a permission: >" + permission + "< (returning PREMISSION_DENIED)");

		return -1; // PackageManager.PERMISSION_DENIED
	}

	public void registerComponentCallbacks(ComponentCallbacks callbacks) {}

	public boolean bindService(Intent dummy, ServiceConnection dummy2, int dummy3) {
		Slog.w(TAG, "bindService("+dummy+", "+dummy2+", "+dummy3+")");
		return false; // maybe?
	}

	public void startActivity(Intent intent) {
		Slog.i(TAG, "startActivity(" + intent + ") called");
		if ("android.intent.action.CHOOSER".equals(intent.getAction())) {
			intent = (Intent) intent.getExtras().get("android.intent.extra.INTENT");
		}
		if (intent.getComponent() == null) {
			Slog.i(TAG, "starting extern activity with intent: " + intent);
			Activity.nativeOpenURI(String.valueOf(intent.getData()));
			return;
		}
		try {
			Class<? extends Activity> cls = Class.forName(intent.getComponent().getClassName()).asSubclass(Activity.class);
			Constructor<? extends Activity> constructor = cls.getConstructor();
			Activity activity = constructor.newInstance();
			activity.intent = intent;
			activity.getWindow().native_window = this_application.native_window;
			Activity.nativeStartActivity(activity);
		} catch (ClassNotFoundException | NoSuchMethodException | SecurityException | InstantiationException | IllegalAccessException | IllegalArgumentException | InvocationTargetException e) {
			e.printStackTrace();
		}
	}

	public final TypedArray obtainStyledAttributes(AttributeSet set, int[] attrs) {
		return getTheme().obtainStyledAttributes(set, attrs, 0, 0);
	}
	public final TypedArray obtainStyledAttributes(AttributeSet set, int[] attrs, int defStyleAttr, int defStyleRes) {
		return getTheme().obtainStyledAttributes(set, attrs, defStyleAttr, defStyleRes);
	}
	public final TypedArray obtainStyledAttributes(int resid, int[] attrs) {
		return getTheme().obtainStyledAttributes(resid, attrs);
	}
	public final TypedArray obtainStyledAttributes(int[] attrs) {
		return getTheme().obtainStyledAttributes(attrs);
	}

	public void setTheme(int resId) {
		theme.applyStyle(resId, true);
	}

	public final CharSequence getText(int resId) {
		return getResources().getText(resId);
	}

	public final Drawable getDrawable(int resId) {
		return getResources().getDrawable(resId);
	}

	public boolean isRestricted() {return false;}

	public File getDatabasePath(String dbName) {
		File databaseDir = new File(getDataDirFile(), "databases");
		if (!databaseDir.exists())
			databaseDir.mkdirs();
		return new File(databaseDir, dbName);
	}

	public void sendBroadcast(Intent intent) {}

	public boolean stopService(Intent intent) {return false;}

	public void unbindService(ServiceConnection serviceConnection) {}
}
