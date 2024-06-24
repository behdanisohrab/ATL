package android.view.animation;

public class Animation {

	public interface AnimationListener {
		public void onAnimationEnd(Animation animation);
		public void onAnimationRepeat(Animation animation);
		public void onAnimationStart(Animation animation);
	}

	public void setDuration(long durationMillis) {}

	public void setInterpolator(Interpolator i) {}

	public void cancel() {}

	public void setFillBefore(boolean dummy) {}
	public void setFillAfter(boolean dummy) {}

	public void setStartOffset(long offset) {}

	public void setAnimationListener(AnimationListener l) {
		l.onAnimationEnd(this); // FIXME
	}

	public void setRepeatCount(int count) {}
}
