package android.widget;

import android.util.AttributeSet;
import android.content.Context;

import android.view.ViewGroup;
import android.view.View;
import android.view.ViewGroup.LayoutParams;

public class LinearLayout extends ViewGroup {

	boolean orientation;

	public LinearLayout(AttributeSet attrs) {
		super(attrs);

		native_constructor(attrs);
	}

	public LinearLayout(Context context) {
		super(context);

		native_constructor(context);
	}

	private native void native_constructor(AttributeSet attrs);
	private native void native_constructor(Context context);

	@Override
	public native void addView(View child, int index, ViewGroup.LayoutParams params);
	@Override
	public native void removeView(View view);
	@Override
	public native void removeAllViews();

	public native void setOrientation(int orientation);
	public void setWeightSum(float weightSum) {}

	public static class LayoutParams extends ViewGroup.LayoutParams {
        public LayoutParams(int width, int height) {
			super(width, height);
		}

		public LayoutParams(int width, int height, float weight) {
			super(width, height, weight);
		}
	}
}
