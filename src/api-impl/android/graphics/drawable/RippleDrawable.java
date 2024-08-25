package android.graphics.drawable;

import android.content.res.ColorStateList;

public class RippleDrawable extends LayerDrawable {

	public RippleDrawable(ColorStateList colorStateList, Drawable drawable, Drawable drawable2) {
		super(new Drawable[] {drawable});
	}

}
