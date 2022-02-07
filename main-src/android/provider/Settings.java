package android.provider;

import android.content.ContentResolver;
import android.util.AndroidException;

public class Settings {
	public static final class Secure {
		public static String getString(ContentResolver content_resolver, String key) {
			switch(key) {
				case "android_id":
					return "_totally_an_androidID"; // TODO: is this a good ID? :P
				default:
					System.out.println("!!!! Settings$Secure.getString: unknown key: >"+key+"<");
					return "NOTICEME";
			}
		}
		public static int getInt(ContentResolver content_resolver, String key) {
			switch(key) {
				case "limit_ad_tracking":
					return 1; // obviously, duh
				default:
					System.out.println("!!!! Settings$Secure.getInt: unknown key: >"+key+"<");
					return -1;
			}
		}
	}

	public static class SettingNotFoundException extends AndroidException {}
}
