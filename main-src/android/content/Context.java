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

import android.view.WindowManager;
import android.view.WindowManagerImpl;
import android.text.ClipboardManager;
import android.hardware.SensorManager;
import android.net.ConnectivityManager;
import android.app.KeyguardManager;
import android.telephony.TelephonyManager;
import android.media.AudioManager;

import java.io.File;

public class Context extends Object {
    private final static String TAG = "Context";

	static AssetManager assets;
	static DisplayMetrics dm;
	static Configuration config;
	static Resources r;

	File data_dir = null;
	File prefs_dir = null;
	File files_dir = null;
	File cache_dir = null;

	static {
		assets = new AssetManager();
		dm = new DisplayMetrics();
		config = new Configuration();
		r = new Resources(assets, dm, config);
	}

	public Context() {
		System.out.println("new Context! this one is: " + this);
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

	// these may not look like typical stubs, but they definitely are stubs
	public final TypedArray obtainStyledAttributes (AttributeSet set, int[] attrs) { return new TypedArray(r, new int[1000], new int[1000], 0); }
	public final TypedArray obtainStyledAttributes (AttributeSet set, int[] attrs, int defStyleAttr, int defStyleRes) { return new TypedArray(r, new int[1000], new int[1000], 0); }
	public final TypedArray obtainStyledAttributes (int resid, int[] attrs) { return new TypedArray(r, new int[1000], new int[1000], 0); }
	public final TypedArray obtainStyledAttributes (int[] attrs) { return new TypedArray(r, new int[1000], new int[1000], 0); }
}
