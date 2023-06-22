package android.text;

import java.util.regex.Pattern;

public class TextUtils {
	// unchanged from android source

	/* split */

	private static String[] EMPTY_STRING_ARRAY = new String[] {};

	public static String[] split(String text, String expression) {
		if (text.length() == 0) {
			return EMPTY_STRING_ARRAY;
		} else {
			return text.split(expression, -1);
		}
	}

	public static String[] split(String text, Pattern pattern) {
		if (text.length() == 0) {
			return EMPTY_STRING_ARRAY;
		} else {
			return pattern.split(text, -1);
		}
	}

	/* join */

	public static String join(CharSequence delimiter, Object[] tokens) {
		StringBuilder sb = new StringBuilder();
		boolean firstTime = true;
		for (Object token : tokens) {
			if (firstTime) {
				firstTime = false;
			} else {
				sb.append(delimiter);
			}
			sb.append(token);
		}
		return sb.toString();
	}

	public static String join(CharSequence delimiter, Iterable tokens) {
		StringBuilder sb = new StringBuilder();
		boolean firstTime = true;
		for (Object token : tokens) {
			if (firstTime) {
				firstTime = false;
			} else {
				sb.append(delimiter);
			}
			sb.append(token);
		}
		return sb.toString();
	}
	// end of unchanged from android source

	public static CharSequence join(Iterable<CharSequence> list) {
		final CharSequence delimiter = ","; // ????
		return join(delimiter, list);
	}

	public static boolean isEmpty(CharSequence str) {
		return str.equals(""); // presumably?
	}
}
