package android.graphics.drawable;

import java.io.IOException;

import org.xmlpull.v1.XmlPullParser;
import org.xmlpull.v1.XmlPullParserException;

import com.android.internal.R;

import android.content.res.Resources;
import android.content.res.Resources.Theme;
import android.content.res.TypedArray;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Rect;
import android.graphics.RectF;
import android.util.AttributeSet;

public class BitmapDrawable extends Drawable {

	private Bitmap bitmap;

	public BitmapDrawable() {
	}

	public BitmapDrawable(Resources res, Bitmap bitmap) {
		this.bitmap = bitmap;
		this.paintable = native_paintable_from_pixbuf(bitmap.pixbuf);
	}

	public Bitmap getBitmap() {
		return bitmap;
	}

	public void inflate(Resources r, XmlPullParser parser, AttributeSet attrs, Theme theme)
			throws XmlPullParserException, IOException {
		final TypedArray a = r.obtainAttributes(attrs, R.styleable.BitmapDrawable);
		if (a.hasValue(R.styleable.BitmapDrawable_src)) {
			paintable = a.getDrawable(R.styleable.BitmapDrawable_src).paintable;
		}
		a.recycle();
	}

	static native long native_paintable_from_pixbuf(long pixbuf);
}
