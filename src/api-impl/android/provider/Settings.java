package android.provider;

import android.content.ContentResolver;
import android.net.Uri;
import android.util.AndroidException;

public class Settings {
	public static final class Secure {
		public static String getString(ContentResolver content_resolver, String key) {
			switch (key) {
				case "android_id":
					return "_totally_an_androidID"; // TODO: is this a good ID? :P
				case "advertising_id":
					return "";
				default:
					java.lang.System.out.println("!!!! Settings$Secure.getString: unknown key: >" + key + "<");
					return "NOTICEME";
			}
		}
		public static int getInt(ContentResolver content_resolver, String key) {
			switch (key) {
				case "limit_ad_tracking":
					return 1; // obviously, duh
				default:
					java.lang.System.out.println("!!!! Settings$Secure.getInt: unknown key: >" + key + "<");
					return -1;
			}
		}
	}
	public static final class System {
		public static final Uri CONTENT_URI = Uri.parse("content://settings/system");

		public static final Uri DEFAULT_NOTIFICATION_URI = getUriFor("notification_sound");

		public static final Uri DEFAULT_RINGTONE_URI = getUriFor("ringtone");

		public static int getInt(ContentResolver cr, String key, int def) {
			int ret = getInt(cr, key);
			if (ret != -1) {
				return ret;
			} else {
				return def; // FIXME
			}
		}

		public static int getInt(ContentResolver cr, String key) {
			switch (key) {
				case "accelerometer_rotation":
					return 0; // degrees? no clue
				case "always_finish_activities":
					return 0; // we certainly don't aggressively kill activities :P
				default:
					java.lang.System.out.println("!!!! Settings$System.getInt: unknown key: >" + key + "<");
					return 0; // TODO: should be -1 probably?
			}
		}

		public static Uri getUriFor(String name) {
			return Uri.withAppendedPath(CONTENT_URI, name);
		}

		public static float getFloat(ContentResolver cr, String key, float def) {
			return 0.0f;
		}
	}

	public static final class Global {

		public static int getInt(ContentResolver cr, String key, int def) {
			switch (key) {
				default:
					java.lang.System.out.println("!!!! Settings$Global.getInt: unknown key: >" + key + "<");
					return def;
			}
		}

		public static float getFloat(ContentResolver cr, String key, float def) {
			switch (key) {
				default:
					java.lang.System.out.println("!!!! Settings$Global.getFloat: unknown key: >" + key + "<");
					return def;
			}
		}
	}

	public static class SettingNotFoundException extends AndroidException {}
}
