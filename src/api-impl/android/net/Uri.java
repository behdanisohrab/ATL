package android.net;

import java.net.URI;
import libcore.net.UriCodec;
import java.io.UnsupportedEncodingException;
import java.nio.charset.StandardCharsets;

public class Uri {

	public static final Uri EMPTY = new Uri();

	private URI uri;

	public static Uri parse(String s) {
		Uri ret = new Uri();
		try {
			ret.uri = URI.create(s);
		} catch (IllegalArgumentException e) {
		}
		return ret;
	}

	public static String decode(String s) {
		if (s == null) {
			return null;
		}
		return UriCodec.decode(s, false, StandardCharsets.UTF_8, false);
	}

	public static String encode(String s) {
		return encode(s, null);
	}

	/**
	 * -- FROM ANDROID, Licensed under the Apache License, Version 2.0 --
	 * Returns true if the given character is allowed.
	 *
	 * @param c character to check
	 * @param allow characters to allow
	 * @return true if the character is allowed or false if it should be
	 *  encoded
	 */
	private final static int NOT_FOUND = -1;
	private static boolean isAllowed(char c, String allow) {
		return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || "_-!.~'()*".indexOf(c) != NOT_FOUND || (allow != null && allow.indexOf(c) != NOT_FOUND);
	}

	/**
	 * -- FROM ANDROID, Licensed under the Apache License, Version 2.0 --
	 * Encodes characters in the given string as '%'-escaped octets
	 * using the UTF-8 scheme. Leaves letters ("A-Z", "a-z"), numbers
	 * ("0-9"), and unreserved characters ("_-!.~'()*") intact. Encodes
	 * all other characters with the exception of those specified in the
	 * allow argument.
	 *
	 * @param s string to encode
	 * @param allow set of additional characters to allow in the encoded form,
	 *  null if no characters should be skipped
	 * @return an encoded version of s suitable for use as a URI component,
	 *  or null if s is null
	 */
	private static final char[] HEX_DIGITS = "0123456789ABCDEF".toCharArray();
	private static final String DEFAULT_ENCODING = "UTF-8";
	public static String encode(String s, String allow) {
		if (s == null) {
			return null;
		}

		// Lazily-initialized buffers.
		StringBuilder encoded = null;

		int oldLength = s.length();

		// This loop alternates between copying over allowed characters and
		// encoding in chunks. This results in fewer method calls and
		// allocations than encoding one character at a time.
		int current = 0;
		while (current < oldLength) {
			// Start in "copying" mode where we copy over allowed chars.

			// Find the next character which needs to be encoded.
			int nextToEncode = current;
			while (nextToEncode < oldLength && isAllowed(s.charAt(nextToEncode), allow)) {
				nextToEncode++;
			}

			// If there's nothing more to encode...
			if (nextToEncode == oldLength) {
				if (current == 0) {
					// We didn't need to encode anything!
					return s;
				} else {
					// Presumably, we've already done some encoding.
					encoded.append(s, current, oldLength);
					return encoded.toString();
				}
			}

			if (encoded == null) {
				encoded = new StringBuilder();
			}

			if (nextToEncode > current) {
				// Append allowed characters leading up to this point.
				encoded.append(s, current, nextToEncode);
			} else {
				// assert nextToEncode == current
			}

			// Switch to "encoding" mode.

			// Find the next allowed character.
			current = nextToEncode;
			int nextAllowed = current + 1;
			while (nextAllowed < oldLength && !isAllowed(s.charAt(nextAllowed), allow)) {
				nextAllowed++;
			}

			// Convert the substring to bytes and encode the bytes as
			// '%'-escaped octets.
			String toEncode = s.substring(current, nextAllowed);
			try {
				byte[] bytes = toEncode.getBytes(DEFAULT_ENCODING);
				int bytesLength = bytes.length;
				for (int i = 0; i < bytesLength; i++) {
					encoded.append('%');
					encoded.append(HEX_DIGITS[(bytes[i] & 0xf0) >> 4]);
					encoded.append(HEX_DIGITS[bytes[i] & 0xf]);
				}
			} catch (UnsupportedEncodingException e) {
				throw new AssertionError(e);
			}

			current = nextAllowed;
		}

		// Encoded could still be null at this point if s is empty.
		return encoded == null ? s : encoded.toString();
	}

	public Builder buildUpon() {
		return new Builder();
	}

	public static final class Builder {
		public Builder appendQueryParameter(String key, String value) {
			return this;
		}
	}

	public String getScheme() {
		return uri.getScheme();
	}

	public String getPath() {
		return uri.getPath();
	}

	@Override
	public String toString() {
		return String.valueOf(uri);
	}
}
