package android.content;

import android.util.Log;

import android.content.pm.PackageManager;
import android.content.res.AssetManager;
import android.content.res.Configuration;
import android.content.res.Resources;
import android.util.DisplayMetrics;
import android.content.SharedPreferences;
import android.app.SharedPreferencesImpl;

import java.io.File;

public class Context extends Object {
    private final static String TAG = "Context";

	AssetManager assets;
	DisplayMetrics dm;
	Configuration config;
	Resources r;

	File data_dir = null;
	File prefs_dir = null;
	File files_dir = null;

	public Context() {
		assets = new AssetManager();
		dm = new DisplayMetrics();
		config = new Configuration();
		r = new Resources(assets, dm, config);
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
}
