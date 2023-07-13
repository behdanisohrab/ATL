package android.view.animation;

/**
 * An interpolator where the rate of change starts out slowly and
 * and then accelerates.
 *
 */
public class AccelerateInterpolator implements Interpolator {
	private final float mFactor;
	private final double mDoubleFactor;

	public AccelerateInterpolator() {
		mFactor = 1.0f;
		mDoubleFactor = 2.0;
	}

	public float getInterpolation(float input) {
		if (mFactor == 1.0f) {
			return input * input;
		} else {
			return (float)Math.pow(input, mDoubleFactor);
		}
	}

}
