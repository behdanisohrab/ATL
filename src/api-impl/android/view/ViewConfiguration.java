package android.view;

import android.content.Context;

public class ViewConfiguration {
	
	public static ViewConfiguration get(Context context) {
		return new ViewConfiguration();
	}

	public int getScaledTouchSlop() {
		return 0;
	}

	public int getScaledMaximumFlingVelocity() {
		return 0;
	}

	public int getScaledMinimumFlingVelocity() {
		return 0;
	}

	public static int getTapTimeout() {
		return 0;
	}

	public static int getLongPressTimeout() {
		return 0;
	}

	public int getScaledPagingTouchSlop(){
		return 0;
	}

	public boolean hasPermanentMenuKey() {
		return false;
	}

	public static int getDoubleTapTimeout() {
		return 0;
	}

	public int getScaledDoubleTapSlop() {
		return 0;
	}
}
