package android.widget;

import android.util.AttributeSet;
import android.content.Context;

import android.view.ViewGroup;
import android.view.View;
import android.view.ViewGroup.LayoutParams;

public class ScrollView extends ViewGroup {
	public ScrollView(AttributeSet attrs) {
		super(attrs);

		native_constructor(attrs);
	}

	public ScrollView(Context context) {
		super(context);

		native_constructor(context);
	}

	private native void native_constructor(AttributeSet attrs);
	private native void native_constructor(Context context);

	@Override
	public native void addView(View child, int index, LayoutParams params);
	@Override
	public native void removeView(View view);
	@Override
	public native void removeAllViews();

	protected void onScrollChanged(int x, int y, int oldx, int oldy) {}
	public void setFillViewport(boolean fillViewport) {}
}
