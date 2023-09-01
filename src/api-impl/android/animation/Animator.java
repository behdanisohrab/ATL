package android.animation;

public class Animator {

	public interface AnimatorListener {
		public abstract void onAnimationEnd (Animator animation);
	}

	public void setTarget(Object target) {}

	public void start() {}

	public void addListener(AnimatorListener listener) {
		listener.onAnimationEnd(Animator.this);
	}
	
}
