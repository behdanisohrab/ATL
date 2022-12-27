package android.content;

import android.util.Log;

import android.content.pm.PackageManager;
import android.content.pm.ApplicationInfo;
import android.content.res.AssetManager;
import android.content.res.Configuration;
import android.content.res.Resources;
import android.content.res.TypedArray;
import android.content.Intent;
import android.content.BroadcastReceiver;

import android.util.AttributeSet;
import android.util.DisplayMetrics;
import android.content.SharedPreferences;
import android.app.SharedPreferencesImpl;
import android.os.Looper;
import android.os.PowerManager;
import android.app.Application;

import android.view.WindowManager;
import android.view.WindowManagerImpl;
import android.content.ClipboardManager;
import android.hardware.SensorManager;
import android.net.ConnectivityManager;
import android.app.KeyguardManager;
import android.telephony.TelephonyManager;
import android.media.AudioManager;
import android.app.ActivityManager;
import android.hardware.usb.UsbManager;
import android.os.Vibrator;
import android.hardware.display.DisplayManager;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;

public class Context extends Object {
	private final static String TAG = "Context";

	public static final int MODE_PRIVATE = 0;
	public static final String LOCATION_SERVICE = "location";
	public static final String AUDIO_SERVICE = "audio";
	public static final String DISPLAY_SERVICE = "display";
	public static final String MEDIA_ROUTER_SERVICE = "media_router";

	static AssetManager assets;
	static DisplayMetrics dm;
	static Configuration config;
	static Resources r;
	static ApplicationInfo application_info;

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
		this_application = new Application(); // TODO: the application context is presumably not identical to the Activity context, what is the difference for us though?
		application_info = new ApplicationInfo();
	}

	public Context() {
		System.out.println("new Context! this one is: " + this);
	}

	public int checkPermission (String permission, int pid, int uid) {
		return getPackageManager().checkPermission(permission, "dummy");
	}

	public Resources.Theme getTheme() {
		return r.newTheme();
	}

	public ApplicationInfo getApplicationInfo () {
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
			default:
				System.out.println("!!!!!!! getSystemService: case >"+name+"< is not implemented yet");
				return null;
		}
	}

	public Intent registerReceiver(BroadcastReceiver receiver, IntentFilter filter) {
		return new Intent();
	}

	public Looper getMainLooper() {
		System.out.println("returning the main Looper, most definitely doing just that!");
		return new Looper();
	}

	public String getPackageName() {
		return "com.example.demo_app";
	}

	public String getPackageCodePath() {
		return apk_path;
	}

	public final String getString(int resId) {
		return r.getString(resId);
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
		if(data_dir == null) {
			data_dir = android.os.Environment.getExternalStorageDirectory();
		}
		return data_dir;
	}

	public File getFilesDir() {
		if (files_dir == null) {
			files_dir = new File(getDataDirFile(), "files");
		}
		if (!files_dir.exists()) {
			if(!files_dir.mkdirs()) {
				if (files_dir.exists()) {
					// spurious failure; probably racing with another process for this app
					return files_dir;
				}
				Log.w(TAG, "Unable to create files directory " + files_dir.getPath());
				return null;
			}
		}
		return files_dir;
	}

	public File getExternalFilesDir(String type) {
		return getFilesDir();
	}

	public File getObbDir() {
	if (obb_dir == null) {
			obb_dir = new File(getDataDirFile(), "obb");
		}
		return obb_dir;
	}

	public File[] getObbDirs() {
		return new File[]{getObbDir()};
	}

	// FIXME: should be something like /tmp/cache, but may need to create that directory
	public File getCacheDir() {
			if (cache_dir == null) {
				cache_dir = new File("/tmp/");
			}
			return cache_dir;
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
		System.out.println("\n\n...> getSharedPreferences("+name+",)\n\n");
		File prefsFile = getSharedPrefsFile(name);
		return new SharedPreferencesImpl(prefsFile, mode);
	}

	public ClassLoader getClassLoader() {
		// not perfect, but it's what we use for now as well, and it works
		return ClassLoader.getSystemClassLoader();
	}

	public ComponentName startService(Intent service) {
		return new ComponentName("","");
	}

	// FIXME - it should be *trivial* to do actually implement this
	public FileInputStream openFileInput(String name) {
		return null;
	}

	public FileOutputStream openFileOutput(String name, int mode) throws java.io.FileNotFoundException {
		System.out.println("openFileOutput called for: '"+name+"'");
		return new FileOutputStream(android.os.Environment.getExternalStorageDirectory().getPath() + "/files/" + name);
	}

	public int checkCallingOrSelfPermission(String permission) {
		System.out.println("!!! app wants to know if it has a permission: >"+permission+"<");

		return -1; // PackageManager.PERMISSION_DENIED
	}

	// these may not look like typical stubs, but they definitely are stubs
	public final TypedArray obtainStyledAttributes (AttributeSet set, int[] attrs) { return new TypedArray(r, new int[1000], new int[1000], 0); }
	public final TypedArray obtainStyledAttributes (AttributeSet set, int[] attrs, int defStyleAttr, int defStyleRes) { return new TypedArray(r, new int[1000], new int[1000], 0); }
	public final TypedArray obtainStyledAttributes (int resid, int[] attrs) { return new TypedArray(r, new int[1000], new int[1000], 0); }
	public final TypedArray obtainStyledAttributes (int[] attrs) { return new TypedArray(r, new int[1000], new int[1000], 0); }
}
