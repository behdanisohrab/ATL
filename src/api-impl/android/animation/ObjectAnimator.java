package android.animation;

public class ObjectAnimator extends ValueAnimator {

	public String getPropertyName() {
		return null;
	}

	public static ObjectAnimator ofFloat(Object target, String propertyName, float... values) {
		return new ObjectAnimator();
	}

	public ObjectAnimator setDuration(long duration) {return this;}

}
