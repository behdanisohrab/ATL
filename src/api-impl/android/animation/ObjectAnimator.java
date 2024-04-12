package android.animation;

import android.graphics.Path;
import android.util.Property;

public class ObjectAnimator extends ValueAnimator {

	public String getPropertyName() {
		return null;
	}

	public static ObjectAnimator ofFloat(Object target, String xPropertyName, String yPropertyName, Path path) {
		return new ObjectAnimator();
	}

	public static <T> ObjectAnimator ofFloat(T target, Property<T, Float> xProperty, Property<T, Float> yProperty, Path path) {
		return new ObjectAnimator();
	}

	public static <T> ObjectAnimator ofFloat(T target, Property<T, Float> property, float... values) {
		return new ObjectAnimator();
	}

	public static ObjectAnimator ofFloat(Object target, String propertyName, float... values) {
		return new ObjectAnimator();
	}

	public static <T> ObjectAnimator ofInt(T target, String propertyName, int... values) {
		return new ObjectAnimator();
	}

	public ObjectAnimator setDuration(long duration) {return this;}

	public void setAutoCancel(boolean autoCancel) {}

}
