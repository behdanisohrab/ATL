package android.view;

import android.content.Context;

public class ScaleGestureDetector {

	public interface OnScaleGestureListener {}

	public ScaleGestureDetector(Context context, OnScaleGestureListener listener) {}

	public void setQuickScaleEnabled(boolean enabled) {}

	public boolean onTouchEvent(MotionEvent event) {
		return false;
	}
}
