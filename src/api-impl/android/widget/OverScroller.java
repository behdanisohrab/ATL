package android.widget;

import android.content.Context;
import android.view.ViewGroup;
import android.view.animation.Interpolator;

public class OverScroller extends ViewGroup {

	public OverScroller(Context context, Interpolator interpolator) {
		super(context);
	}

	public void abortAnimation () {}

	public void startScroll(int startX, int startY, int dx, int dy, int duration) {}
}
