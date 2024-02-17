package android.graphics.drawable;

import java.io.IOException;
import java.io.InputStream;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.Arrays;

import org.xmlpull.v1.XmlPullParser;
import org.xmlpull.v1.XmlPullParserException;

import android.content.res.ColorStateList;
import android.content.res.Resources;
import android.content.res.XmlResourceParser;
import android.content.res.Resources.Theme;
import android.graphics.Canvas;
import android.graphics.ColorFilter;
import android.graphics.PorterDuff;
import android.graphics.Rect;
import android.util.AttributeSet;
import android.util.TypedValue;

public class Drawable {
	public static interface Callback {}

	private Rect mBounds = new Rect();
	private int[] mStateSet = new int[0];
	public long paintable;

	public Drawable() {}

	public Drawable(long paintable) {
		this.paintable = paintable;
	}

	public int getChangingConfigurations() {
		return 0;
	}

	public void setChangingConfigurations(int bitmap) {}

	public ConstantState getConstantState() {
		return null;
	}

	public abstract class ConstantState {

		public abstract Drawable newDrawable(Resources res);

		public abstract Drawable newDrawable();
	}

	public void setBounds(int left, int top, int right, int bottom) {
		mBounds.set(left, top, right, bottom);
	}

	public final Rect getBounds() {
		return mBounds;
	}

	public void draw(Canvas canvas) {}

	public boolean setState(int[] stateSet) {
		if (!Arrays.equals(this.mStateSet, stateSet)) {
			this.mStateSet = stateSet;
			return onStateChange(stateSet);
		}
		return false;
	}

	public int[] getState() {
		return mStateSet;
	}

	public void invalidateSelf() {}

	public void setCallback(Callback callback) {}

	public boolean isVisible() {
		return false;
	}

	public boolean setVisible (boolean visible, boolean restart) {
		return false;
	}

	public void clearColorFilter() {}

	public final int getLevel() {return 0;}
	public final boolean setLevel(int level) {return false;}

	public void setBounds(Rect bounds) {}

	public void setColorFilter(int color, PorterDuff.Mode mode) {}
	public void setColorFilter(ColorFilter filter) {}

	public Drawable mutate() {
		return this;
	}

	public int getIntrinsicWidth() {return 0;}
	public int getIntrinsicHeight() {return 0;}

	public void setTintList (ColorStateList tint) {}

	public void setTint(int tint) {}

	public boolean isStateful() {
		return false;
	}

	public void setTintMode(PorterDuff.Mode tintMode) {}

	public boolean isProjected () {return false;}

	public static Drawable createFromXml(Resources resources, XmlResourceParser parser) throws XmlPullParserException, IOException {
		int type;
		while ((type=parser.next()) != XmlPullParser.START_TAG && type != XmlPullParser.END_DOCUMENT);
		if (type != XmlPullParser.START_TAG)
			throw new XmlPullParserException("No start tag found");
		if ("selector".equals(parser.getName())) {
			StateListDrawable drawable = new StateListDrawable();
			drawable.inflate(resources, parser, parser, null);
			return drawable;
		} else if ("shape".equals(parser.getName())) {
			return new GradientDrawable();
		} else if ("bitmap".equals(parser.getName())) {
			BitmapDrawable drawable = new BitmapDrawable();
			drawable.inflate(resources, parser, parser, null);
			return drawable;
		} else if ("transition".equals(parser.getName())) {
			return new Drawable();
		} else if ("ripple".equals(parser.getName())) {
			// FIXME: the non-pressed state of RippleDrawable should be equivalent to this
			return new ColorDrawable(0);
		}
		return null;
	}

	public static Drawable createFromXmlInner(Resources resources, XmlPullParser parser, AttributeSet attrs, Theme theme) {
		return null;
	}

	public static Drawable createFromResourceStream(Resources resources, TypedValue value, InputStream is, String file,
			Object object) {
		Path path = Paths.get(android.os.Environment.getExternalStorageDirectory().getPath(), file);
		if (!Files.exists(path)) {
			try (InputStream inputStream = ClassLoader.getSystemClassLoader().getResourceAsStream(file)) {
				if (inputStream != null) {
					Files.createDirectories(path.getParent());
					Files.copy(inputStream, path);
				}
			} catch (IOException e) {
				e.printStackTrace();
			}
		}
		long paintable = native_paintable_from_path(path.toString());
		return new Drawable(paintable);
	}

	protected boolean onStateChange(int[] stateSet) {
		return false;
	}

	public void setAlpha(int alpha) {}

	public boolean getPadding(Rect padding) {
		return false;
	}

	protected static native long native_paintable_from_path(String path);
}
