package android.animation;

final class PropertyValuesHolder{}

public class ValueAnimator extends Animator {

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

	public PropertyValuesHolder[] getValues() {
		return null;
	}

	public long getStartDelay() {return 0;}
	public long getDuration() {return 0;}
	public TimeInterpolator getInterpolator() {return null;}
	public int getRepeatCount() {return 0;}
	public int getRepeatMode() {return 0;}
	public void setInterpolator(TimeInterpolator interpolator) {}
	public void setFloatValues(float[] values) {}
	public boolean isRunning() {return false;}
	public void setIntValues(int[] values) {}
	public void setRepeatCount(int value) {}
	public void setRepeatMode(int value) {}

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
