package android.gesture;

import android.content.Context;
import android.view.ViewGroup;

public class GestureOverlayView extends ViewGroup {

	public GestureOverlayView(Context context) {
		super(context);
		haveCustomMeasure = false;  // hack: let it measure 0x0, as GTK doesn't allow events to go through it
	}

	public void setGestureStrokeType(int type) {
	}

	public void setEventsInterceptionEnabled(boolean enabled) {
	}

	public void setGestureVisible(boolean enabled) {
	}

	public void addOnGesturePerformedListener(OnGesturePerformedListener listener) {
	}

	public interface OnGesturePerformedListener {
	}
}
