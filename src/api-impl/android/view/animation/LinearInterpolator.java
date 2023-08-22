package android.view.animation;

import android.animation.TimeInterpolator;

public class LinearInterpolator implements TimeInterpolator {

	@Override
	public float getInterpolation(float input) {
		return input;
	}

}
