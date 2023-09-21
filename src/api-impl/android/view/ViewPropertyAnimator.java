package android.view;

import android.animation.Animator;
import android.animation.TimeInterpolator;

public class ViewPropertyAnimator {

	public void cancel() {}

	public ViewPropertyAnimator setInterpolator(TimeInterpolator interpolator) {
		return this;
	}

	public ViewPropertyAnimator setListener(Animator.AnimatorListener listener) {
		if (listener != null)
			listener.onAnimationEnd(new Animator());
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

	public ViewPropertyAnimator rotation(float rotation) {
		return this;
	}

	public ViewPropertyAnimator translationX(float translationX) {
		return this;
	}

	public ViewPropertyAnimator translationY(float translationY) {
		return this;
	}

	public ViewPropertyAnimator scaleX(float scaleX) {
		return this;
	}

	public ViewPropertyAnimator scaleY(float scaleY) {
		return this;
	}

	public void start() {}
}
