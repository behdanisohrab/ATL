package android.graphics.drawable;

import android.graphics.Canvas;
import android.graphics.Rect;

public abstract class Drawable {

	private Rect mBounds = new Rect();

	public int getChangingConfigurations() {
		return 0;
	}

	public void setChangingConfigurations(int bitmap) {}

	public ConstantState getConstantState() {
		return new ConstantState();
	}
	
	public class ConstantState {
		
	}

	public void setBounds(int left, int top, int right, int bottom) {
		mBounds.set(left, top, right, bottom);
	}

	public final Rect getBounds() {
		return mBounds;
	}

	public abstract void draw(Canvas canvas);
}
