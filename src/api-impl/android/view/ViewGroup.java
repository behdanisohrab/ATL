package android.view;

import android.R;
import android.animation.LayoutTransition;
import android.content.Context;
import android.content.res.TypedArray;
import android.util.AttributeSet;
import java.util.ArrayList;
import java.util.Iterator;

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

	public ViewGroup(Context context, AttributeSet attrs) {
		super(context, attrs);

		children = new ArrayList<View>();
	}

	public ViewGroup(Context context, AttributeSet attrs, int defStyleAttr) {
		super(context, attrs);

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

	public void addView(View child, int index, LayoutParams params) {
		if (params != null) {
			child.setLayoutParams(params);
		}
		child.parent = this;
		children.add(child);
		native_addView(widget, child.widget, index, params);
	}

	public void removeView(View child) {
		child.parent = null;
		children.remove(child);
		native_removeView(widget, child.widget);
	}

	public void removeAllViews() {
		for (Iterator<View> it = children.iterator(); it.hasNext();) {
			View child = it.next();
			child.parent = null;
			it.remove();
			native_removeView(widget, child.widget);
		}
	}

	@Override
	protected native long native_constructor(Context context, AttributeSet attrs);
	protected native void native_addView(long widget, long child, int index, LayoutParams params);
	protected native void native_removeView(long widget, long child);

	public View getChildAt(int index) {
		return children.get(index);
	}

	public void updateViewLayout(View view, ViewGroup.LayoutParams params) {}

	public LayoutParams generateLayoutParams(AttributeSet attrs) {
		return new LayoutParams(getContext(), attrs);
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

	public void setMotionEventSplittingEnabled(boolean enabled) {}

	public void setOnHierarchyChangeListener(OnHierarchyChangeListener listener) {}

	public boolean checkLayoutParams(LayoutParams params) {
		return true;
	}

	public LayoutTransition getLayoutTransition() {return null;}

	public static int getChildMeasureSpec(int spec, int padding, int childDimension) {
		int specMode = MeasureSpec.getMode(spec);
		int specSize = MeasureSpec.getSize(spec);
		int size = Math.max(0, specSize - padding);
		int resultSize = 0;
		int resultMode = 0;
		switch (specMode) {
		// Parent has imposed an exact size on us
		case MeasureSpec.EXACTLY:
			if (childDimension >= 0) {
				resultSize = childDimension;
				resultMode = MeasureSpec.EXACTLY;
			} else if (childDimension == LayoutParams.MATCH_PARENT) {
				// Child wants to be our size. So be it.
				resultSize = size;
				resultMode = MeasureSpec.EXACTLY;
			} else if (childDimension == LayoutParams.WRAP_CONTENT) {
				// Child wants to determine its own size. It can't be
				// bigger than us.
				resultSize = size;
				resultMode = MeasureSpec.AT_MOST;
			}
			break;
		// Parent has imposed a maximum size on us
		case MeasureSpec.AT_MOST:
			if (childDimension >= 0) {
				// Child wants a specific size... so be it
				resultSize = childDimension;
				resultMode = MeasureSpec.EXACTLY;
			} else if (childDimension == LayoutParams.MATCH_PARENT) {
				// Child wants to be our size, but our size is not fixed.
				// Constrain child to not be bigger than us.
				resultSize = size;
				resultMode = MeasureSpec.AT_MOST;
			} else if (childDimension == LayoutParams.WRAP_CONTENT) {
				// Child wants to determine its own size. It can't be
				// bigger than us.
				resultSize = size;
				resultMode = MeasureSpec.AT_MOST;
			}
			break;
		// Parent asked to see how big we want to be
		case MeasureSpec.UNSPECIFIED:
			if (childDimension >= 0) {
				// Child wants a specific size... let them have it
				resultSize = childDimension;
				resultMode = MeasureSpec.EXACTLY;
			} else if (childDimension == LayoutParams.MATCH_PARENT) {
				// Child wants to be our size... find out how big it should
				// be
				resultSize = 0; //View.sUseZeroUnspecifiedMeasureSpec ? 0 : size;
				resultMode = MeasureSpec.UNSPECIFIED;
			} else if (childDimension == LayoutParams.WRAP_CONTENT) {
				// Child wants to determine its own size.... find out how
				// big it should be
				resultSize = 0; //View.sUseZeroUnspecifiedMeasureSpec ? 0 : size;
				resultMode = MeasureSpec.UNSPECIFIED;
			}
			break;
		}
		//noinspection ResourceType
		return MeasureSpec.makeMeasureSpec(resultSize, resultMode);
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

		public LayoutParams(Context context, AttributeSet attrs) {
			TypedArray a = context.obtainStyledAttributes(attrs, R.styleable.ViewGroup_Layout);
			width = a.getLayoutDimension(R.styleable.ViewGroup_Layout_layout_width, "layout_width");
			height = a.getLayoutDimension(R.styleable.ViewGroup_Layout_layout_height, "layout_height");
			this.gravity = attrs.getAttributeIntValue("http://schemas.android.com/apk/res/android", "layout_gravity", -1);
			a.recycle();
		}

		public void setMargins(int left, int top, int right, int bottom) {}

		/**
		 * Used to animate layouts.
		 */
//		public LayoutAnimationController.AnimationParameters layoutAnimationParameters;
	}

	public static class MarginLayoutParams extends ViewGroup.LayoutParams {
		public int leftMargin;
		public int topMargin;
		public int rightMargin;
		public int bottomMargin;

		public MarginLayoutParams() {
			super();
		}

		public MarginLayoutParams(int width, int height) {
			super(width, height);
		}

		public MarginLayoutParams(int width, int height, float weight) {
			super(width, height, weight);
		}

		public MarginLayoutParams(Context context, AttributeSet attributeSet) {
			super(context, attributeSet);
		}
	}

	public interface OnHierarchyChangeListener {

	}
}
