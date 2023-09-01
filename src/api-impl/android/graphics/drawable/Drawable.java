package android.graphics.drawable;

import android.content.res.Resources;
import android.graphics.Canvas;
import android.graphics.ColorFilter;
import android.graphics.PorterDuff;
import android.graphics.Rect;

public abstract class Drawable {
	public static interface Callback {}

	private Rect mBounds = new Rect();
	private int[] mStateSet = new int[0];

	public int getChangingConfigurations() {
		return 0;
	}

	public void setChangingConfigurations(int bitmap) {}

	public ConstantState getConstantState() {
		return null;
	}
	
	public abstract class ConstantState {

		public abstract Drawable newDrawable(Resources res);

		public abstract Drawable newDrawable();
	}

	public void setBounds(int left, int top, int right, int bottom) {
		mBounds.set(left, top, right, bottom);
	}

	public final Rect getBounds() {
		return mBounds;
	}

	public abstract void draw(Canvas canvas);

	public boolean setState(int[] stateSet) {
		this.mStateSet = stateSet;
		return false;
	}

	public int[] getState() {
		return mStateSet;
	}

	public void invalidateSelf() {}

	public void setCallback(Callback callback) {}

	public boolean isVisible() {
		return false;
	}

	public boolean setVisible (boolean visible, boolean restart) {
		return false;
	}

	public void clearColorFilter() {}

	public final int getLevel() {return 0;}
	public final boolean setLevel(int level) {return false;}

	public void setBounds(Rect bounds) {}

	public void setColorFilter(int color, PorterDuff.Mode mode) {}
	public void setColorFilter(ColorFilter filter) {}

	public Drawable mutate() {
		return this;
	}

	public int getIntrinsicWidth() {return 0;}
	public int getIntrinsicHeight() {return 0;}
}
