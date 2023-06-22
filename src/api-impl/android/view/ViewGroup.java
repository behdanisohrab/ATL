package android.view;

import android.content.Context;
import android.util.AttributeSet;
import java.util.ArrayList;

public class ViewGroup extends View implements ViewParent, ViewManager {
	public int id;
	public ArrayList<View> children;

	public ViewGroup() {
		children = new ArrayList<View>();
	}

	public ViewGroup(Context context) {
		super(context);

		children = new ArrayList<View>();
	}

	public ViewGroup(AttributeSet attrs) {
		super(attrs);

		children = new ArrayList<View>();
	}

	public ViewGroup(int _id) { // FIXME
		children = new ArrayList<View>();

		id = _id;
	}

	public void addView(View child) {
		addView(child, -1, null);
	}

	public void addView(View child, int index) {
		addView(child, index, null);
	}

	public void addView(View child, LayoutParams params) {
		addView(child, -1, params);
	}

	public void addView(View child, int width, int height) {
		addView(child, new LayoutParams(width, height));
	}

	public native void addView(View child, int index, LayoutParams params);

	public native void removeView(View view);
	public native void removeAllViews();

	public View getChildAt(int index) {
		return children.get(index);
	}

	public void updateViewLayout(View view, ViewGroup.LayoutParams params) {}

	public LayoutParams generateLayoutParams(AttributeSet attrs) {
		return new LayoutParams(/*getContext(), attrs*/);
	}

	public void bringChildToFront(View child) {
		// TODO: actually implement this (might make sense to implement it in the subclasses instead), when applicable
	}

	/**
	 * Returns the number of children in the group.
	 *
	 * @return a positive integer representing the number of children in
	 *         the group
	 */
	public int getChildCount() {
		return children.size();
	}

	public static class LayoutParams {
		public static final int FILL_PARENT = -1;
		public static final int MATCH_PARENT = -1;
		public static final int WRAP_CONTENT = -2;

		public int width = 0;
		public int height = 0;
		public float weight = 1;
		public int gravity = -1;

		public LayoutParams() {
			// FIXME
		}

		public LayoutParams(int width, int height) {
			this.width = width;
			this.height = height;
		}

		public LayoutParams(int width, int height, float weight) {
			this.width = width;
			this.height = height;
			this.weight = weight;
		}

		public void setMargins(int left, int top, int right, int bottom) {}

		/**
		 * Used to animate layouts.
		 */
//		public LayoutAnimationController.AnimationParameters layoutAnimationParameters;
	}

	public static class MarginLayoutParams extends ViewGroup.LayoutParams {

		public MarginLayoutParams() {
			super();
		}

		public MarginLayoutParams(int width, int height) {
			super(width, height);
		}

		public MarginLayoutParams(int width, int height, float weight) {
			super(width, height, weight);
		}
	}
}
