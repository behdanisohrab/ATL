package android.widget;

import android.content.Context;
import android.view.animation.Interpolator;

public class OverScroller {

	public OverScroller(Context context, Interpolator interpolator) {
	}

	public OverScroller(Context context) {
	}

	public void abortAnimation () {}

	public void startScroll(int startX, int startY, int dx, int dy, int duration) {}

	public boolean computeScrollOffset() {return false;}

	public int getCurrX() {return 0;}
	public int getCurrY() {return 0;}

	public int getFinalX() {return 0;}
	public int getFinalY() {return 0;}
}
