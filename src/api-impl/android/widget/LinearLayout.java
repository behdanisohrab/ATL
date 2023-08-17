package android.widget;

import android.content.Context;
import android.util.AttributeSet;
import android.view.View;
import android.view.ViewGroup;

public class LinearLayout extends ViewGroup {

	boolean orientation;

	public LinearLayout(Context context, AttributeSet attrs) {
		super(context, attrs);
	}

	public LinearLayout(Context context) {
		super(context);
	}

	@Override
	protected native long native_constructor(Context context, AttributeSet attrs);
	@Override
	protected native void native_addView(long widget, long child, int index, ViewGroup.LayoutParams params);
	@Override
	protected native void native_removeView(long widget, long child);

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
