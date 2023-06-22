package android.preference;

import android.app.SharedPreferencesImpl;
import android.content.Context;
import android.content.SharedPreferences;

public class PreferenceManager {
	public static SharedPreferences getDefaultSharedPreferences(Context context) {
		return Context.this_application.getSharedPreferences("default", 0); // is this correct?
	}
}
