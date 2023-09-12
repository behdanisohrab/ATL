package android.os;

import java.util.Set;

import android.util.ArrayMap;
import android.util.Log;

public class BaseBundle {
	protected static final String TAG = "Bundle";

	// Invariant - exactly one of mMap / mParcelledData will be null
	// (except inside a call to unparcel)

	/* package */ ArrayMap<String, Object> mMap = null;

	// Log a message if the value was non-null but not of the expected type
	void typeWarning(String key, Object value, String className,
				 Object defaultValue, ClassCastException e) {
		StringBuilder sb = new StringBuilder();
		sb.append("Key ");
		sb.append(key);
		sb.append(" expected ");
		sb.append(className);
		sb.append(" but value was a ");
		sb.append(value.getClass().getName());
		sb.append(".  The default value ");
		sb.append(defaultValue);
		sb.append(" was returned.");
		Log.w(TAG, sb.toString());
		Log.w(TAG, "Attempt to cast generated internal exception:", e);
	}

	void typeWarning(String key, Object value, String className,
			ClassCastException e) {
		typeWarning(key, value, className, "<null>", e);
	}

	/**
	 * Returns true if the mapping of this Bundle is empty, false otherwise.
	 */
	public boolean isEmpty() {
		return mMap.isEmpty();
	}

	/**
	 * Returns a Set containing the Strings used as keys in this Bundle.
	 *
	 * @return a Set of String keys
	 */
	public Set<String> keySet() {
		return mMap.keySet();
	}

	/**
	 * Returns true if the given key is contained in the mapping
	 * of this Bundle.
	 *
	 * @param key a String key
	 * @return true if the key is part of the mapping, false otherwise
	 */
	public boolean containsKey(String key) {
		return mMap.containsKey(key);
	}

	/**
	 * Returns the value associated with the given key, or null if
	 * no mapping of the desired type exists for the given key or a null
	 * value is explicitly associated with the key.
	 *
	 * @param key a String, or null
	 * @return a String value, or null
	 */
	public String getString(String key) {
		final Object o = mMap.get(key);
		try {
			return (String)o;
		} catch (ClassCastException e) {
			typeWarning(key, o, "String", e);
			return null;
		}
	}

	/**
	 * Returns the value associated with the given key, or defaultValue if
	 * no mapping of the desired type exists for the given key.
	 *
	 * @param key a String, or null
	 * @param defaultValue Value to return if key does not exist
	 * @return the String value associated with the given key, or defaultValue
	 *     if no valid String object is currently mapped to that key.
	 */
	public String getString(String key, String defaultValue) {
		final String s = getString(key);
		return (s == null) ? defaultValue : s;
	}
}
