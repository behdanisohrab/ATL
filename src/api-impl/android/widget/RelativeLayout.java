package android.widget;

import android.util.AttributeSet;
import android.content.Context;

import android.view.ViewGroup;
import android.view.View;
import android.view.ViewGroup.LayoutParams;

public class RelativeLayout extends ViewGroup {

	boolean orientation;

	public RelativeLayout(AttributeSet attrs) {
		super(attrs);

		native_constructor(attrs);
	}

	public RelativeLayout(Context context) {
		super(context);

		native_constructor(context);
	}

	private native void native_constructor(AttributeSet attrs);
	private native void native_constructor(Context context);

	@Override
	public native void addView(View child, int index, ViewGroup.LayoutParams params);

	public static class LayoutParams extends ViewGroup.LayoutParams {
        public LayoutParams(int width, int height) {
			super(width, height);
		}

		public LayoutParams(int width, int height, float weight) {
			super(width, height, weight);
		}

		public void addRule(int verb) {}
		public void addRule(int verb, int anchor) {}
	}
}
