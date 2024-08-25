package android.graphics.drawable;

import android.graphics.Rect;

public class InsetDrawable extends Drawable {

	private Drawable drawable;

	public InsetDrawable(Drawable drawable, int insetLeft, int insetTop, int insetRight, int insetBottom) {
		super();
		this.drawable = drawable;
	}

	public InsetDrawable(Drawable drawable, int inset) {
		super();
		this.drawable = drawable;
	}

	public boolean getPadding(Rect padding) { return false; }

	public Drawable getDrawable() {
		return drawable;
	}

}
