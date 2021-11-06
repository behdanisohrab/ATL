package android.view;

import android.util.AttributeSet;
import android.content.Context;

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

	public native void addView(View child, int index, LayoutParams params);

    public native void removeView(View view);/* {
		System.out.println("NOT_IMPLEMENTED: ViewGroup.removeView: Gtk4 doesn't have a generic function for removing a child of GtkView, so you must override this function in the actual widget's class");
	}*/
	public native void removeAllViews();/* {
		System.out.println("NOT_IMPLEMENTED: ViewGroup.removeAllViews: Gtk4 doesn't have a generic function for removing a child of GtkView, so you must override this function in the actual widget's class");
		new Exception().printStackTrace();
	}*/

	public View getChildAt(int index) {
		return children.get(index);
	}

    public void updateViewLayout(View view, ViewGroup.LayoutParams params) {}

	public LayoutParams generateLayoutParams(AttributeSet attrs) {
		return new LayoutParams(/*getContext(), attrs*/);
	}

	public static class LayoutParams {
		public static final int FILL_PARENT = -1;
		public static final int MATCH_PARENT = -1;
		public static final int WRAP_CONTENT = -2;

		public int width;
		public int height;
		public float weight;

		public LayoutParams() {
			//FIXME
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
}
