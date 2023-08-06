package android.graphics.drawable;

import android.content.res.Resources;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Rect;
import android.graphics.RectF;

public class BitmapDrawable extends Drawable {

	private Bitmap bitmap;

	public BitmapDrawable(Resources res, Bitmap bitmap) {
		this.bitmap = bitmap;
	}

	@Override
	public void draw(Canvas canvas) {
		canvas.drawBitmap(bitmap, new Rect(0, 0, bitmap.getWidth(), bitmap.getWidth()), new RectF(getBounds()), null);
	}

}
