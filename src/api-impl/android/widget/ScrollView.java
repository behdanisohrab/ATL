package android.widget;

import android.content.Context;
import android.util.AttributeSet;
import android.view.ViewGroup;

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
	protected native void native_addView(long widget, long child, int index, LayoutParams params);
	@Override
	protected native void native_removeView(long widget, long child);

	public void setFillViewport(boolean fillViewport) {}
}
