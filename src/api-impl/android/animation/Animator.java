package android.animation;

public class Animator {

	public interface AnimatorListener {
		public abstract void onAnimationEnd (Animator animation);
	}

	private AnimatorListener listener;

	public void setTarget(Object target) {}

	public void start() {
		if (listener != null)
			listener.onAnimationEnd(this);
	}

	public void addListener(AnimatorListener listener) {
		this.listener = listener;
	}
	
}
