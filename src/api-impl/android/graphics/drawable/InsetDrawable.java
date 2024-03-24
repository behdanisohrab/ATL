package android.graphics.drawable;

import android.graphics.Rect;

public class InsetDrawable extends Drawable {

	public InsetDrawable(Drawable drawable, int insetLeft, int insetTop, int insetRight, int insetBottom) {
		super();
	}

	public boolean getPadding(Rect padding) { return false; }
	
}
