package android.view;

public class VelocityTracker {

	public static VelocityTracker obtain() {
		return new VelocityTracker();
	}

	public void addMovement(MotionEvent event) {}

	public void recycle() {}

	public void computeCurrentVelocity(int units, float maxVelocity) {}

	public float getXVelocity(int id) {return 0.f;}
	public float getYVelocity(int id) {return 0.f;}

	public void clear() {}
}
