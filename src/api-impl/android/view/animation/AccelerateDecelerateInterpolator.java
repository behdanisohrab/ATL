package android.view.animation;

import android.animation.TimeInterpolator;

public class AccelerateDecelerateInterpolator implements TimeInterpolator {

	@Override
	public float getInterpolation(float input) {
		return input;
	}

}
