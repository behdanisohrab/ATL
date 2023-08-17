package android.widget;

import android.content.Context;
import android.util.AttributeSet;
import android.view.View;
import android.view.ViewGroup;
import android.view.ViewGroup.LayoutParams;

public class ScrollView extends ViewGroup {
	public ScrollView(Context context, AttributeSet attrs) {
		super(context, attrs);
	}

	public ScrollView(Context context) {
		super(context);
	}

	@Override
	protected native long native_constructor(Context context, AttributeSet attrs);
	@Override
	protected native void native_addView(long widget, long child, int index, ViewGroup.LayoutParams params);
	@Override
	protected native void native_removeView(long widget, long child);

	protected void onScrollChanged(int x, int y, int oldx, int oldy) {}
	public void setFillViewport(boolean fillViewport) {}
}
