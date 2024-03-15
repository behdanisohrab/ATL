package android.view.animation;

public class Animation {

	public interface AnimationListener {}

	public void setDuration(long durationMillis) {}

	public void setInterpolator(Interpolator i) {}

	public void cancel() {}

	public void setFillBefore(boolean dummy) {}
	public void setFillAfter(boolean dummy) {}
}
