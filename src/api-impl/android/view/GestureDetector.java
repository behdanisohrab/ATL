package android.view;

import android.content.Context;
import android.os.Handler;

public class GestureDetector {

	public GestureDetector(Context context, OnGestureListener listener, Handler handler) {}

	public GestureDetector(Context context, OnGestureListener listener) {}

	public boolean onTouchEvent(MotionEvent event) {
		return false;
	}

	public interface OnGestureListener {
	}

	public interface OnDoubleTapListener {
	}
	
	public static class SimpleOnGestureListener implements OnGestureListener {

	}
}
