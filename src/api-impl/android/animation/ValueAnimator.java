package android.animation;

public class ValueAnimator {

	public static ValueAnimator ofFloat(float... values) {
		return new ValueAnimator();
	}

	public ValueAnimator setDuration(long duration) {
		return this;
	}

	public void addUpdateListener(AnimatorUpdateListener listener) {}

	public static long getFrameDelay() {
		return 20; // 20ms frame interval
	}

	/**
	 * Implementors of this interface can add themselves as update listeners
	 * to an <code>ValueAnimator</code> instance to receive callbacks on every animation
	 * frame, after the current frame's values have been calculated for that
	 * <code>ValueAnimator</code>.
	 */
	public static interface AnimatorUpdateListener {
		/**
		 * <p>Notifies the occurrence of another frame of the animation.</p>
		 *
		 * @param animation The animation which was repeated.
		 */
		void onAnimationUpdate(ValueAnimator animation);
	}
}
