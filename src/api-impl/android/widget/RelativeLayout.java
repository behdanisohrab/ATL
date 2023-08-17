package android.widget;

import android.content.Context;
import android.util.AttributeSet;
import android.view.View;
import android.view.ViewGroup;
import android.view.ViewGroup.LayoutParams;

public class RelativeLayout extends ViewGroup {

	boolean orientation;

	public RelativeLayout(Context context, AttributeSet attrs) {
		super(context, attrs);
	}

	public RelativeLayout(Context context) {
		super(context);
	}

	@Override
	protected native long native_constructor(Context context, AttributeSet attrs);
	@Override
	protected native void native_addView(long widget, long child, int index, ViewGroup.LayoutParams params);
	@Override
	protected native void native_removeView(long widget, long child);

	public static class LayoutParams extends ViewGroup.LayoutParams {
		public LayoutParams(int width, int height) {
			super(width, height);
		}

		public LayoutParams(int width, int height, float weight) {
			super(width, height, weight);
		}

		public LayoutParams(ViewGroup.MarginLayoutParams source) {
			super(source.width, source.height, source.weight);
		}

		public void addRule(int verb) {}
		public void addRule(int verb, int anchor) {}
	}
}
