package android.graphics.drawable;

import java.io.IOException;

import org.xmlpull.v1.XmlPullParser;
import org.xmlpull.v1.XmlPullParserException;

import com.android.internal.R;

import android.content.res.Resources;
import android.content.res.Resources.Theme;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.content.res.TypedArray;
import android.util.AttributeSet;

public class VectorDrawable extends Drawable {

	public VectorDrawable() {
		super();
	}

	public void inflate(Resources r, XmlPullParser parser, AttributeSet attrs, Theme theme) throws XmlPullParserException, IOException {
		final int innerDepth = parser.getDepth() + 1;
		int type;
		TypedArray a = r.obtainAttributes(attrs, R.styleable.VectorDrawable);
		int tint = a.getColor(R.styleable.VectorDrawable_tint, 0);
		float viewportWidth = a.getFloat(R.styleable.VectorDrawable_viewportWidth, 24);
		float viewportHeight = a.getFloat(R.styleable.VectorDrawable_viewportHeight, 24);
		a.recycle();
		StringBuilder sb = new StringBuilder();
		sb.append(String.format("<svg id=\"vector\" xmlns=\"http://www.w3.org/2000/svg\" width=\"%f\" height=\"%f\" viewBox=\"0 0 %f %f\">",
				viewportWidth, viewportHeight, viewportWidth, viewportHeight));
		while ((type = parser.next()) != XmlPullParser.END_DOCUMENT
				&& (parser.getDepth() >= innerDepth
				|| type != XmlPullParser.END_TAG)) {

			if (type == XmlPullParser.START_TAG && parser.getName().equals("path")) {
				a = r.obtainAttributes(attrs, R.styleable.VectorDrawablePath);
				String pathData = a.getString(R.styleable.VectorDrawablePath_pathData);
				int fillColor = tint != 0 ? tint : a.getColor(R.styleable.VectorDrawablePath_fillColor, 0);
				int strokeColor = tint != 0 ? tint : a.getColor(R.styleable.VectorDrawablePath_strokeColor, 0);
				float strokeWidth = a.getFloat(R.styleable.VectorDrawablePath_strokeWidth, 0);
				a.recycle();
				sb.append(String.format("<path fill=\"#%06x%02x\" stroke=\"#%06x%02x\" stroke-width=\"%f\" d=\"%s\"/>",
						fillColor & 0xFFFFFF, (fillColor >> 24) & 0xFF, strokeColor & 0xFFFFFF, (strokeColor >> 24) & 0xFF, strokeWidth, pathData));
			}
		}
		sb.append("</svg>");
		String svg = sb.toString();
		byte[] bytes = svg.getBytes();
		Bitmap bm = BitmapFactory.decodeByteArray(bytes, 0, bytes.length);
		this.paintable = bm.getTexture();
	}

	@Override
	public int getIntrinsicWidth() {
		return 24; // FIXME
	}

	@Override
	public int getIntrinsicHeight() {
		return 24; // FIXME
	}
}
