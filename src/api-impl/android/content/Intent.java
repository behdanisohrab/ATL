package android.content;

import android.net.Uri;
import android.os.Bundle;
import android.os.Parcelable;
import java.io.Serializable;
import java.util.HashMap;
import java.util.Map;

public class Intent {
	private ComponentName component;
	private Bundle extras = new Bundle();

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
		extras.putParcelable(name, value);
		return this;
	}

	public Intent putExtra(String name, long[] value) {
		extras.putLongArray(name, value);
		return this;
	}

	public Intent putExtra(String name, byte value) {
		extras.putByte(name, value);
		return this;
	}

	public Intent putExtra(String name, double[] value) {
		extras.putDoubleArray(name, value);
		return this;
	}

	public Intent putExtra(String name, CharSequence value) {
		extras.putCharSequence(name, value);
		return this;
	}

	public Intent putExtra(String name, boolean[] value) {
		extras.putBooleanArray(name, value);
		return this;
	}

	public Intent putExtra(String name, int value) {
		extras.putInt(name, value);
		return this;
	}

	public Intent putExtra(String name, char[] value) {
		extras.putCharArray(name, value);
		return this;
	}

	public Intent putExtra(String name, byte[] value) {
		extras.putByteArray(name, value);
		return this;
	}

	public Intent putExtra(String name, Parcelable[] value) {
		extras.putParcelableArray(name, value);
		return this;
	}

	public Intent putExtra(String name, Bundle value) {
		extras.putBundle(name, value);
		return this;
	}

	public Intent putExtra(String name, CharSequence[] value) {
		extras.putCharSequenceArray(name, value);
		return this;
	}

	public Intent putExtra(String name, float[] value) {
		extras.putFloatArray(name, value);
		return this;
	}

	public Intent putExtra(String name, double value) {
		extras.putDouble(name, value);
		return this;
	}

	public Intent putExtra(String name, int[] value) {
		extras.putIntArray(name, value);
		return this;
	}

	public Intent putExtra(String name, String[] value) {
		extras.putStringArray(name, value);
		return this;
	}

	public Intent putExtra(String name, short[] value) {
		extras.putShortArray(name, value);
		return this;
	}

	public Intent putExtra(String name, boolean value) {
		extras.putBoolean(name, value);
		return this;
	}

	public Intent putExtra(String name, String value) {
		extras.putString(name, value);
		return this;
	}

	public Intent putExtra(String name, long value) {
		extras.putLong(name, value);
		return this;
	}

	public Intent putExtra(String name, char value) {
		extras.putChar(name, value);
		return this;
	}

	public Intent putExtra(String name, Serializable value) {
		extras.putSerializable(name, value);
		return this;
	}

	public Intent putExtra(String name, float value) {
		extras.putFloat(name, value);
		return this;
	}

	public Intent putExtra(String name, short value) {
		extras.putShort(name, value);
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

	public Bundle getExtras() {
		return extras;
	}

	@Override
	public String toString() {
		return "Intent [component=" + component + ", extras=" + extras + "]";
	}

}
