package android.widget;

import android.util.AttributeSet;
import android.content.Context;

import android.view.ViewGroup;
import android.view.View;
import android.view.ViewGroup.LayoutParams;

public class FrameLayout extends ViewGroup {

	public FrameLayout(AttributeSet attrs) {
		super(attrs);

		native_constructor(attrs);
	}

	public FrameLayout(Context context) {
		super(context);

		native_constructor(context);
	}

	public native void native_constructor(AttributeSet attrs);
	public native void native_constructor(Context context);

	@Override
	public native void addView(View child, int index, ViewGroup.LayoutParams params);

	public void addView(View child, int index) {
		addView(child, index, null);
	}

	public static class LayoutParams extends ViewGroup.MarginLayoutParams {
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
