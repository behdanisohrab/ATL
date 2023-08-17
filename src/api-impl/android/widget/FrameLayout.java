package android.widget;

import android.content.Context;
import android.util.AttributeSet;
import android.view.View;
import android.view.ViewGroup;
import android.view.ViewGroup.LayoutParams;

public class FrameLayout extends ViewGroup {

	public FrameLayout(Context context, AttributeSet attrs) {
		super(context, attrs);

		native_constructor(attrs);
	}

	public FrameLayout(Context context) {
		super(context);

		native_constructor(context);
	}

	public FrameLayout(Context context, AttributeSet attrs, int defStyleAttr) {
		super(context, attrs, defStyleAttr);

		native_constructor(attrs);
	}

	public native void native_constructor(AttributeSet attrs);
	public native void native_constructor(Context context);

	@Override
	public native void addView(View child, int index, ViewGroup.LayoutParams params);

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
