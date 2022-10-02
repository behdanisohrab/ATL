package android.content;

import android.net.Uri;
import android.os.Bundle;
import android.os.Parcelable;

import java.io.Serializable;

public class Intent {
	public Intent () {}
	public Intent (Intent o) {}
	public Intent (String action) {}
	public Intent (String action, Uri uri) {}
	public Intent (Context packageContext, Class<?> cls) {}
	public Intent (String action, Uri uri, Context packageContext, Class<?> cls) {}

	public Intent setPackage(String packageName) {
		return this; //??
	}

	public Intent putExtra (String name, Parcelable value) {
		return this; //??
	}

	public Intent putExtra(String name, long[] value) {
		return this; //??
	}

	public Intent putExtra(String name, byte value) {
		return this; //??
	}

	public Intent putExtra(String name, double[] value) {
		return this; //??
	}

	public Intent putExtra(String name, CharSequence value) {
		return this; //??
	}

	public Intent putExtra(String name, boolean[] value) {
		return this; //??
	}

	public Intent putExtra(String name, int value) {
		return this; //??
	}

	public Intent putExtra(String name, char[] value) {
		return this; //??
	}

	public Intent putExtra(String name, byte[] value) {
		return this; //??
	}

	public Intent putExtra(String name, Parcelable[] value) {
		return this; //??
	}

	public Intent putExtra(String name, Bundle value) {
		return this; //??
	}

	public Intent putExtra(String name, CharSequence[] value) {
		return this; //??
	}

	public Intent putExtra(String name, float[] value) {
		return this; //??
	}

	public Intent putExtra(String name, double value) {
		return this; //??
	}

	public Intent putExtra(String name, int[] value) {
		return this; //??
	}

	public Intent putExtra(String name, String[] value) {
		return this; //??
	}

	public Intent putExtra(String name, short[] value) {
		return this; //??
	}

	public Intent putExtra(String name, boolean value) {
		return this; //??
	}

	public Intent putExtra(String name, String value) {
		return this; //??
	}

	public Intent putExtra(String name, long value) {
		return this; //??
	}

	public Intent putExtra(String name, char value) {
		return this; //??
	}

	public Intent putExtra(String name, Serializable value) {
		return this; //??
	}

	public Intent putExtra(String name, float value) {
		return this; //??
	}

	public Intent putExtra(String name, short value) {
		return this; //??
	}

	public Intent setClass (Context packageContext, Class<?> cls) {
		return this; //??
	}

}
