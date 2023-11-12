package android.widget;

import android.content.Context;
import android.util.AttributeSet;
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

	public native void setOrientation(int orientation);
	public void setWeightSum(float weightSum) {}

	@Override
	public LayoutParams generateLayoutParams(AttributeSet attrs) {
		return new LayoutParams(getContext(), attrs);
	}

	public static class LayoutParams extends ViewGroup.MarginLayoutParams {
		public LayoutParams (Context c, AttributeSet attrs) {
			super(c, attrs);
		}

		public LayoutParams(int width, int height) {
			super(width, height);
		}

		public LayoutParams(int width, int height, float weight) {
			super(width, height, weight);
		}
	}
}
