package android.content;

import android.util.Log;

import android.content.pm.PackageManager;
import android.content.res.AssetManager;
import android.content.res.Configuration;
import android.content.res.Resources;
import android.content.res.TypedArray;
import android.util.AttributeSet;
import android.util.DisplayMetrics;
import android.content.SharedPreferences;
import android.app.SharedPreferencesImpl;
import android.os.Looper;
import android.app.Application;

import android.view.WindowManager;
import android.view.WindowManagerImpl;
import android.text.ClipboardManager;
import android.hardware.SensorManager;
import android.net.ConnectivityManager;
import android.app.KeyguardManager;
import android.telephony.TelephonyManager;
import android.media.AudioManager;
import android.app.ActivityManager;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;

public class Context extends Object {
    private final static String TAG = "Context";

	static AssetManager assets;
	static DisplayMetrics dm;
	static Configuration config;
	static Resources r;

	public /*← FIXME?*/ static Application this_application;

	File data_dir = null;
	File prefs_dir = null;
	File files_dir = null;
	File cache_dir = null;

	static {
		assets = new AssetManager();
		dm = new DisplayMetrics();
		config = new Configuration();
		r = new Resources(assets, dm, config);
		this_application = new Application(); // TODO: the application context is presumably not identical to the Activity context, what is the difference for us though?
	}

	public Context() {
		System.out.println("new Context! this one is: " + this);
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
			default:
				System.out.println("!!!!!!! getSystemService: case >"+name+"< is not implemented yet");
				return null;
		}
	}

    public Looper getMainLooper() {
		System.out.println("returning the main Looper, most definitely doing just that!");
		return new Looper();
	}

	public String getPackageName() {
		return "com.example.demo_app";
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
			data_dir = new File("data/");
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
		return new FileOutputStream("data/files/" + name);
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