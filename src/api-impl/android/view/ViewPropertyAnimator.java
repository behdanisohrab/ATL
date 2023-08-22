package android.view;

import android.animation.TimeInterpolator;

public class ViewPropertyAnimator {

	public void cancel() {}

	public ViewPropertyAnimator setInterpolator(TimeInterpolator interpolator) {
		return this;
	}
}
