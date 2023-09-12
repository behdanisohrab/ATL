package android.animation;

import android.content.Context;

public class AnimatorInflater {

	public static Animator loadAnimator(Context context, int resId) {
		return new ObjectAnimator();
	}

	public static StateListAnimator loadStateListAnimator(Context context, int resId) {
		return new StateListAnimator();
	}
	
}
