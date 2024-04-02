package android.view;

import android.R;
import android.animation.LayoutTransition;
import android.content.Context;
import android.content.res.TypedArray;
import android.util.AttributeSet;
import java.util.ArrayList;
import java.util.Iterator;

public class ViewGroup extends View implements ViewParent, ViewManager {
	public ArrayList<View> children;
	private OnHierarchyChangeListener onHierarchyChangeListener;

	public ViewGroup(Context context) {
		this(context, null);
	}

	public ViewGroup(Context context, AttributeSet attrs) {
		this(context, attrs, 0);
	}

	public ViewGroup(Context context, AttributeSet attrs, int defStyleAttr) {
		this(context, attrs, defStyleAttr, 0);
	}

	public ViewGroup(Context context, AttributeSet attrs, int defStyleAttr, int defStyleRes) {
		super(context, attrs, defStyleAttr);

		children = new ArrayList<View>();
	}

	public void addView(View child) {
		addView(child, -1);
	}

	public void addView(View child, int index) {
		LayoutParams params = child.getLayoutParams();
		if (params == null) {
			params = generateDefaultLayoutParams();
		}
		addView(child, index, params);
	}

	public void addView(View child, LayoutParams params) {
		addView(child, -1, params);
	}

	public void addView(View child, int width, int height) {
		final LayoutParams params = generateDefaultLayoutParams();
		params.width = width;
		params.height = height;
		addView(child, params);
	}

	protected void addViewInternal(View child, int index, LayoutParams params) {
		if (child.parent == this)
			return;
		if (!checkLayoutParams(params))
			params = generateLayoutParams(params);

		child.parent = this;
		child.setLayoutParams(params);
		if (index < 0)
			index = children.size();
		children.add(index, child);
		native_addView(widget, child.widget, index, params);
		if (isAttachedToWindow())
			child.onAttachedToWindow();
		if (onHierarchyChangeListener != null)
			onHierarchyChangeListener.onChildViewAdded(this, child);

		requestLayout();
	}

	public void addView(View child, int index, LayoutParams params) {
		addViewInternal(child, index, params);
	}

	protected boolean addViewInLayout(View child, int index, LayoutParams params) {
		addViewInternal(child, index, params);
		return true;
	}

	// This internal method is used to share code between removeView and removeViewInLayout.
	// Reusing removeView in removeViewInLayout is not possible, because e.g.
	// ViewPager overrides removeView to call removeViewInLayout
	protected void removeViewInternal(View child) {
		if (child.parent != this)
			return;
		child.parent = null;
		children.remove(child);
		native_removeView(widget, child.widget);
		if (onHierarchyChangeListener != null) {
			onHierarchyChangeListener.onChildViewRemoved(this, child);
		}
	}

	public void removeView(View child) {
		removeViewInternal(child);
	}

	public void removeViewInLayout(View child) {
		removeViewInternal(child);
	}

	public void removeViewAt(int index) {
		removeView(children.get(index));
	}

	public void removeAllViews() {
		for (Iterator<View> it = children.iterator(); it.hasNext();) {
			View child = it.next();
			child.parent = null;
			it.remove();
			native_removeView(widget, child.widget);
			if (onHierarchyChangeListener != null) {
				onHierarchyChangeListener.onChildViewRemoved(this, child);
			}
		}
	}

	public void detachViewFromParent(int index) {
		removeViewAt(index);
	}

	public void attachViewToParent(View view, int index, LayoutParams params) {
		addViewInternal(view, index, params);
	}

	protected void removeDetachedView(View child, boolean animate) {
	}

	protected native void native_addView(long widget, long child, int index, LayoutParams params);
	protected native void native_removeView(long widget, long child);

	public View getChildAt(int index) {
		try {
			return children.get(index);
		} catch (IndexOutOfBoundsException e) {
			return null;
		}
	}

	public int indexOfChild(View child) {
		return children.indexOf(child);
	}

	public void updateViewLayout(View view, ViewGroup.LayoutParams params) {
		view.setLayoutParams(params);
		view.requestLayout();
	}

	public LayoutParams generateLayoutParams(AttributeSet attrs) {
		return new LayoutParams(getContext(), attrs);
	}
	protected LayoutParams generateLayoutParams(ViewGroup.LayoutParams p) {
		return p;
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

	public void setOnHierarchyChangeListener(OnHierarchyChangeListener listener) {
		this.onHierarchyChangeListener = listener;
	}

	protected boolean checkLayoutParams(LayoutParams params) {
		return true;
	}

	public LayoutTransition getLayoutTransition() { return null; }

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
					resultSize = 0; // View.sUseZeroUnspecifiedMeasureSpec ? 0 : size;
					resultMode = MeasureSpec.UNSPECIFIED;
				} else if (childDimension == LayoutParams.WRAP_CONTENT) {
					// Child wants to determine its own size.... find out how
					// big it should be
					resultSize = 0; // View.sUseZeroUnspecifiedMeasureSpec ? 0 : size;
					resultMode = MeasureSpec.UNSPECIFIED;
				}
				break;
		}
		// noinspection ResourceType
		return MeasureSpec.makeMeasureSpec(resultSize, resultMode);
	}

	protected void measureChildWithMargins(View child,
					       int parentWidthMeasureSpec, int widthUsed,
					       int parentHeightMeasureSpec, int heightUsed) {
		final MarginLayoutParams lp = (MarginLayoutParams)child.getLayoutParams();
		final int childWidthMeasureSpec = getChildMeasureSpec(parentWidthMeasureSpec,
								      /*mPaddingLeft + mPaddingRight +*/ lp.leftMargin + lp.rightMargin + widthUsed, lp.width);
		final int childHeightMeasureSpec = getChildMeasureSpec(parentHeightMeasureSpec,
								       /*mPaddingTop + mPaddingBottom +*/ lp.topMargin + lp.bottomMargin + heightUsed, lp.height);
		child.measure(childWidthMeasureSpec, childHeightMeasureSpec);
	}

	protected void measureChild(View child, int parentWidthMeasureSpec,
				    int parentHeightMeasureSpec) {
		final LayoutParams lp = child.getLayoutParams();
		final int childWidthMeasureSpec = getChildMeasureSpec(parentWidthMeasureSpec,
								      /*mPaddingLeft + mPaddingRight*/ 0, lp.width);
		final int childHeightMeasureSpec = getChildMeasureSpec(parentHeightMeasureSpec,
								       /*mPaddingTop + mPaddingBottom*/ 0, lp.height);
		child.measure(childWidthMeasureSpec, childHeightMeasureSpec);
	}

	protected void measureChildren(int widthMeasureSpec, int heightMeasureSpec) {
		for (View child: children) {
			measureChild(child, widthMeasureSpec, heightMeasureSpec);
		}
	}

	public void setAddStatesFromChildren(boolean addsStates) {}

	public View getFocusedChild() { return null; }

	public int getDescendantFocusability() { return 0; }

	public void startViewTransition(View view) {}
	public void endViewTransition(View view) {}

	protected LayoutParams generateDefaultLayoutParams() {
		return new LayoutParams(LayoutParams.WRAP_CONTENT, LayoutParams.WRAP_CONTENT);
	}

	public void focusableViewAvailable(View v) {}

	@Override
	public void setGravity(int gravity) {
		super.setGravity(gravity);
		// update children as necessary
		for (View child: children) {
			LayoutParams params = child.getLayoutParams();
			if (params.gravity == -1)
				child.setLayoutParams(params);
		}
	}

	protected void setChildrenDrawingOrderEnabled(boolean enabled) {}

	public void requestDisallowInterceptTouchEvent(boolean disallowIntercept) {}

	@Override
	protected void onAttachedToWindow() {
		super.onAttachedToWindow();
		for (View child: children) {
			child.onAttachedToWindow();
		}
	}

	protected boolean isChildrenDrawingOrderEnabled() { return false; }

	@Override
	public boolean onStartNestedScroll(View child, View target, int nestedScrollAxes) {
		return false;
	}

	@Override
	public boolean onNestedPreFling(View target, float velocityX, float velocityY) {
		return false;
	}

	@Override
	public boolean onNestedFling(View target, float velocityX, float velocityY, boolean consumed) {
		return false;
	}

	public void setClipToPadding(boolean clipToPadding) {}

	public View findViewById(int id) {
		if (this.id == id)
			return this;
		for (View child: children) {
			View result = child.findViewById(id);
			if (result != null)
				return result;
		}
		return null;
	}

	public static class LayoutParams {
		public static final int FILL_PARENT = -1;
		public static final int MATCH_PARENT = -1;
		public static final int WRAP_CONTENT = -2;

		public int width = 0;
		public int height = 0;
		public float weight = 0;
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
			setBaseAttributes(a, R.styleable.ViewGroup_Layout_layout_width, R.styleable.ViewGroup_Layout_layout_height);
			this.gravity = attrs.getAttributeIntValue("http://schemas.android.com/apk/res/android", "layout_gravity", -1);
			a.recycle();
		}

		protected void setBaseAttributes(TypedArray a, int widthAttr, int heightAttr) {
			width = a.getLayoutDimension(widthAttr, "layout_width");
			height = a.getLayoutDimension(heightAttr, "layout_height");
		}

		public void resolveLayoutDirection(int layoutDirection) {}

		/**
		 * Used to animate layouts.
		 */
		//public LayoutAnimationController.AnimationParameters layoutAnimationParameters;
	}

	public static class MarginLayoutParams extends ViewGroup.LayoutParams {
		public int leftMargin;
		public int topMargin;
		public int rightMargin;
		public int bottomMargin;

		public MarginLayoutParams() {
			super();
		}

		public MarginLayoutParams(LayoutParams params) {
			super();
			width = params.width;
			height = params.height;
		}

		public MarginLayoutParams(int width, int height) {
			super(width, height);
		}

		public MarginLayoutParams(int width, int height, float weight) {
			super(width, height, weight);
		}

		public MarginLayoutParams(Context context, AttributeSet attributeSet) {
			super(context, attributeSet);
			TypedArray a = context.obtainStyledAttributes(attributeSet, R.styleable.ViewGroup_MarginLayout);
			leftMargin = a.getDimensionPixelSize(R.styleable.ViewGroup_MarginLayout_layout_marginLeft, 0);
			topMargin = a.getDimensionPixelSize(R.styleable.ViewGroup_MarginLayout_layout_marginTop, 0);
			rightMargin = a.getDimensionPixelSize(R.styleable.ViewGroup_MarginLayout_layout_marginRight, 0);
			bottomMargin = a.getDimensionPixelSize(R.styleable.ViewGroup_MarginLayout_layout_marginBottom, 0);
			a.recycle();
		}

		public void setMargins(int left, int top, int right, int bottom) {
			leftMargin = left;
			topMargin = top;
			rightMargin = right;
			bottomMargin = bottom;
		}

		public int getMarginStart() {
			return leftMargin;
		}

		public int getMarginEnd() {
			return rightMargin;
		}

		public void setMarginStart(int marginStart) {
			leftMargin = marginStart;
		}
		public void setMarginEnd(int marginEnd) {
			rightMargin = marginEnd;
		}

		public int getLayoutDirection() { return LAYOUT_DIRECTION_LTR; }
	}

	public interface OnHierarchyChangeListener {
		public void onChildViewAdded(View parent, View child);
		public void onChildViewRemoved(View parent, View child);
	}
}
