package android.widget;

import android.content.Context;
import android.util.AttributeSet;
import android.view.ViewGroup;

public class FrameLayout extends ViewGroup {

	public FrameLayout(Context context, AttributeSet attrs) {
		super(context, attrs);
	}

	public FrameLayout(Context context) {
		super(context);
	}

	public FrameLayout(Context context, AttributeSet attrs, int defStyleAttr) {
		super(context, attrs, defStyleAttr);
	}

	@Override
	protected native long native_constructor(Context context, AttributeSet attrs);

	@Override
	public LayoutParams generateLayoutParams(AttributeSet attrs) {
		return new LayoutParams(getContext(), attrs);
	}

	@Override
	protected LayoutParams generateDefaultLayoutParams() {
		return new LayoutParams(LayoutParams.MATCH_PARENT, LayoutParams.MATCH_PARENT);
	}

	public static class LayoutParams extends ViewGroup.MarginLayoutParams {
		public LayoutParams (Context c, AttributeSet attrs) {
			super(c, attrs);
		}

		public LayoutParams(int width, int height) {
			super(width, height);
		}

		public LayoutParams(int width, int height, float weight) {
			super(width, height, weight);
		}

		public LayoutParams(int width, int height, int gravity) {
			this.width = width;
			this.height = height;
			this.gravity = gravity;
		}

		public LayoutParams (ViewGroup.LayoutParams params) {
			this.width = params.width;
			this.height = params.height;
		}
	}
}
