package android.content;

import android.net.Uri;
import android.os.Bundle;
import android.os.Parcelable;
import java.io.Serializable;
import java.util.HashMap;
import java.util.Map;

public class Intent {
	private ComponentName component;
	private Map<String, Object> extras = new HashMap<>();

	public Intent() {}
	public Intent(Intent o) {}
	public Intent(String action) {}
	public Intent(String action, Uri uri) {}
	public Intent(Context packageContext, Class<?> cls) {
		setClass(packageContext, cls);
	}
	public Intent(String action, Uri uri, Context packageContext, Class<?> cls) {}

	public Intent addFlags(int flags) {
		return this; //??
	}

	public Intent setFlags(int flags) {
		return this; //??
	}
	public Intent setPackage(String packageName) {
		return this; //??
	}

	public Intent putExtra(String name, Parcelable value) {
		extras.put(name, value);
		return this;
	}

	public Intent putExtra(String name, long[] value) {
		extras.put(name, value);
		return this;
	}

	public Intent putExtra(String name, byte value) {
		extras.put(name, value);
		return this;
	}

	public Intent putExtra(String name, double[] value) {
		extras.put(name, value);
		return this;
	}

	public Intent putExtra(String name, CharSequence value) {
		extras.put(name, value);
		return this;
	}

	public Intent putExtra(String name, boolean[] value) {
		extras.put(name, value);
		return this;
	}

	public Intent putExtra(String name, int value) {
		extras.put(name, value);
		return this;
	}

	public Intent putExtra(String name, char[] value) {
		extras.put(name, value);
		return this;
	}

	public Intent putExtra(String name, byte[] value) {
		extras.put(name, value);
		return this;
	}

	public Intent putExtra(String name, Parcelable[] value) {
		extras.put(name, value);
		return this;
	}

	public Intent putExtra(String name, Bundle value) {
		extras.put(name, value);
		return this;
	}

	public Intent putExtra(String name, CharSequence[] value) {
		extras.put(name, value);
		return this;
	}

	public Intent putExtra(String name, float[] value) {
		extras.put(name, value);
		return this;
	}

	public Intent putExtra(String name, double value) {
		extras.put(name, value);
		return this;
	}

	public Intent putExtra(String name, int[] value) {
		extras.put(name, value);
		return this;
	}

	public Intent putExtra(String name, String[] value) {
		extras.put(name, value);
		return this;
	}

	public Intent putExtra(String name, short[] value) {
		extras.put(name, value);
		return this;
	}

	public Intent putExtra(String name, boolean value) {
		extras.put(name, value);
		return this;
	}

	public Intent putExtra(String name, String value) {
		extras.put(name, value);
		return this;
	}

	public Intent putExtra(String name, long value) {
		extras.put(name, value);
		return this;
	}

	public Intent putExtra(String name, char value) {
		extras.put(name, value);
		return this;
	}

	public Intent putExtra(String name, Serializable value) {
		extras.put(name, value);
		return this;
	}

	public Intent putExtra(String name, float value) {
		extras.put(name, value);
		return this;
	}

	public Intent putExtra(String name, short value) {
		extras.put(name, value);
		return this;
	}

	public Intent setClass(Context packageContext, Class<?> cls) {
		setComponent(new ComponentName(packageContext, cls));
		return this;
	}

	public String getStringExtra(String name) {
		return (String)extras.get(name);
	}

	public Uri getData() {
		return null;
	}

	public boolean getBooleanExtra(String name, boolean defaultValue) {
		return defaultValue;
	}

	public String getAction() {
		return null;
	}

	public Bundle getBundleExtra(String name) {
		return (Bundle)extras.get(name);
	}

	public Intent setComponent(ComponentName component) {
		this.component = component;
		return this;
	}
	
	public ComponentName getComponent() {
		return component;
	}

	public boolean hasExtra(String name) {
		return extras.containsKey(name);
	}

	public Serializable getSerializableExtra(String name) {
		return (Serializable)extras.get(name);
	}

	@Override
	public String toString() {
		return "Intent [component=" + component + ", extras=" + extras + "]";
	}

}
