package android.preference;

import android.content.Context;
import android.content.SharedPreferences;
import android.app.SharedPreferencesImpl;

public class PreferenceManager {
	public static SharedPreferences getDefaultSharedPreferences (Context context) {
		return Context.this_application.getSharedPreferences("default", 0); // is this correct?
	}
}
