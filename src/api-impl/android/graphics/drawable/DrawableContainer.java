package android.graphics.drawable;

import android.content.res.Resources;
import android.graphics.Canvas;

public class DrawableContainer extends Drawable {

	private DrawableContainerState state;
	private int curIndex = -1;

	protected native long native_constructor();
	protected native void native_selectChild(long container, long child);

	public DrawableContainer() {
		paintable = native_constructor();
	}

	public boolean selectDrawable(int idx) {
		if (idx >= 0 && idx < state.childCount && idx != curIndex && state.drawables[idx] != null) {
			curIndex = idx;
			native_selectChild(paintable, state.drawables[idx].paintable);
			invalidateSelf();
			return true;
		}
		return false;
	}

	protected void setConstantState(DrawableContainerState state) {
		this.state = state;
	}

	public static class DrawableContainerState {

		private Drawable drawables[] = new Drawable[10];
		private int childCount = 0;

		public DrawableContainerState(DrawableContainerState orig, DrawableContainer owner, Resources res) {
		}

		public int getCapacity() {
			return drawables.length;
		}

		public int getChildCount() {
			return childCount;
		}

		public Drawable getChild(int idx) {
			return drawables[idx];
		}

		public int addChild(Drawable dr) {
			if (childCount >= drawables.length) {
				growArray(drawables.length, drawables.length * 2);
			}
			drawables[childCount] = dr;
			return childCount++;
		}

		public void growArray(int oldSize, int newSize) {
				Drawable[] newDrawables = new Drawable[newSize];
				System.arraycopy(drawables, 0, newDrawables, 0, oldSize);
				drawables = newDrawables;
		}
	}

	@Override
	public void draw(Canvas canvas) {
		state.drawables[curIndex].draw(canvas);
	}

	@Override
	public int getIntrinsicHeight() {
		return state.drawables[curIndex].getIntrinsicHeight();
	}

	@Override
	public int getIntrinsicWidth() {
		return state.drawables[curIndex].getIntrinsicWidth();
	}

	@Override
	public void setBounds(int left, int top, int right, int bottom) {
		state.drawables[curIndex].setBounds(left, top, right, bottom);
	}

}
