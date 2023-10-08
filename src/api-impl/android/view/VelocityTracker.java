package android.view;

public class VelocityTracker {

	public static VelocityTracker obtain() {
		return new VelocityTracker();
	}

	public void addMovement(MotionEvent event) {}

	public void recycle() {}
}
