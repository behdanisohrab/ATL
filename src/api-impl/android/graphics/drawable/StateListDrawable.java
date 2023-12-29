package android.graphics.drawable;

import java.io.IOException;

import org.xmlpull.v1.XmlPullParser;
import org.xmlpull.v1.XmlPullParserException;

import android.content.res.Resources;
import android.content.res.Resources.Theme;
import android.graphics.Canvas;
import android.util.AttributeSet;
import android.util.Slog;

public class StateListDrawable extends Drawable {

	@Override
	public void draw(Canvas canvas) {
		// TODO Auto-generated method stub
		throw new UnsupportedOperationException("Unimplemented method 'draw'");
	}

	public void addState(int[] stateSet, Drawable drawable) {}

    public void inflate(Resources resources, XmlPullParser parser, AttributeSet attrs, Theme theme) throws XmlPullParserException, IOException {
		while (parser.next() != XmlPullParser.END_DOCUMENT) {
			if ("item".equals(parser.getName())) {
				int resid = attrs.getAttributeResourceValue("http://schemas.android.com/apk/res/android", "drawable", -1);
				Drawable drawable = resources.getDrawable(resid, theme);
				this.paintable = drawable.paintable;
				Slog.i("StateListDrawable", "item resid = " + resid + ", paintable = " + drawable.paintable);
			}
		}
    }
	
}
