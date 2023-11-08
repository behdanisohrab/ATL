package android.graphics.drawable;

import android.graphics.Canvas;
import android.graphics.Rect;

public class InsetDrawable extends Drawable {

	public InsetDrawable(Drawable drawable, int insetLeft, int insetTop, int insetRight, int insetBottom) {
		super();
	}

	@Override
	public void draw(Canvas canvas) {
		// TODO Auto-generated method stub
		throw new UnsupportedOperationException("Unimplemented method 'draw'");
	}

	public boolean getPadding(Rect padding) { return false; }
	
}
