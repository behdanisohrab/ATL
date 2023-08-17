package android.widget;

import android.content.Context;
import android.util.AttributeSet;
import android.view.View;
import android.view.ViewGroup;
import android.view.ViewGroup.LayoutParams;

public class FrameLayout extends ViewGroup {

	public FrameLayout(Context context, AttributeSet attrs) {
		super(context, attrs);
	}

	public FrameLayout(Context context) {
		super(context);
	}

	public FrameLayout(Context context, AttributeSet attrs, int defStyleAttr) {
		super(context, attrs, defStyleAttr);
	}

	@Override
	protected native long native_constructor(Context context, AttributeSet attrs);
	@Override
	protected native void native_addView(long widget, long child, int index, ViewGroup.LayoutParams params);
	@Override
	protected native void native_removeView(long widget, long child);

	public void addView(View child, int index) {
		addView(child, index, null);
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

		public LayoutParams(int width, int height, int gravity) {
			this.width = width;
			this.height = height;
			this.gravity = gravity;
		}
	}
}
