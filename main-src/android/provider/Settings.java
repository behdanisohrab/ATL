package android.provider;

import android.content.ContentResolver;

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
	}
}
