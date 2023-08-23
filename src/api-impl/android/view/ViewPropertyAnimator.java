package android.view;

import android.animation.Animator;
import android.animation.TimeInterpolator;

public class ViewPropertyAnimator {

	public void cancel() {}

	public ViewPropertyAnimator setInterpolator(TimeInterpolator interpolator) {
		return this;
	}

	public ViewPropertyAnimator setListener(Animator.AnimatorListener listener) {
		return this;
	}

	public ViewPropertyAnimator alpha(float alpha) {
		return this;
	}

	public ViewPropertyAnimator setDuration(long duration) {
		return this;
	}

	public ViewPropertyAnimator setStartDelay(long duration) {
		return this;
	}

	public void start() {}
}
