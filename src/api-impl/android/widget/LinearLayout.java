package android.widget;

import android.content.Context;
import android.util.AttributeSet;
import android.view.View;
import android.view.ViewGroup;

public class LinearLayout extends ViewGroup {

	public LinearLayout(Context context, AttributeSet attrs) {
		super(context, attrs);
	}

	public LinearLayout(Context context) {
		super(context);
	}

	@Override
	protected native long native_constructor(Context context, AttributeSet attrs);

	public native void setOrientation(int orientation);
	public native int getOrientation();
	protected native void native_setHomogenousWeight(long widget, boolean homogenous);
	public void setWeightSum(float weightSum) {}

	@Override
	protected void addViewInternal(View child, int index, ViewGroup.LayoutParams params) {
		super.addViewInternal(child, index, params);
		// check if all children have the same weight and set GtkBox to homogeneous if so
		float weight = params.weight;
		for (int i = 0; i < children.size(); i++) {
			if (children.get(i).getLayoutParams().weight != weight) {
				weight = 0;
				break;
			}
		}
		native_setHomogenousWeight(widget, weight > 0);
	}

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
