package android.animation;

public class AnimatorSet extends Animator {

	public class Builder {

		public Builder with(Animator animator) {
			return this;
		}
	}

	public Builder play(Animator animator) {
		return new Builder();
	}

	public void setInterpolator(TimeInterpolator value) {}

	public void playSequentially(Animator[] animators) {}

}
