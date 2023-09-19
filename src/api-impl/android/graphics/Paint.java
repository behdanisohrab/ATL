package android.graphics;

public class Paint {
	public long skia_paint = 0;
	private Typeface typeface = null;
	public long skia_font = 0;

	public Paint() {
		skia_paint = native_constructor();
	}

	public Paint (int flags) {
		this();
		setFlags(flags);
	}

	public Paint(Paint paint) {
		/* TODO: use sk_paint_clone */
		this();
		setColor(paint.getColor());
	}

	public void setColor(int color) {
		native_set_color(skia_paint, color);
	}

	public int getColor() {
		return native_get_color(skia_paint);
	}

	public void setAntiAlias(boolean aa) {}
	public void setStrokeWidth(float width) {}
	public void setTextSize(float size) {
		if(skia_font == 0)
			skia_font = native_create_font();

		native_set_text_size(skia_font, size);
	}

	public Typeface setTypeface(Typeface typeface) {
		this.typeface = typeface;
		if(skia_font == 0)
			skia_font = native_create_font();

		if (typeface != null)
			native_set_typeface(skia_font, typeface.skia_typeface);
		return this.typeface;
	}
	public void getTextBounds(String text, int start, int end, Rect bounds) {}
	public void getTextBounds(char[] text, int index, int count, Rect bounds) {}
	public void setFlags(int flags) {}
	public void setFilterBitmap(boolean filter) {}
	public void setStyle(Style style) {}
	public float ascent() {
		if(skia_font == 0)
			return 0;
		return native_ascent(skia_font);
	}

	public float measureText(char[] text, int index, int count) { return 10; }
	public float measureText(String text, int start, int end) { return 10; }
	public float measureText(String text) {
		return native_measure_text(skia_font, text, 0, text.length(), skia_paint);
	}
	public float measureText(CharSequence text, int start, int end) { return 10; }

	public enum Style {
		/**
		 * Geometry and text drawn with this style will be filled, ignoring all
		 * stroke-related settings in the paint.
		 */
		FILL(0),
		/**
		 * Geometry and text drawn with this style will be stroked, respecting
		 * the stroke-related fields on the paint.
		 */
		STROKE(1),
		/**
		 * Geometry and text drawn with this style will be both filled and
		 * stroked at the same time, respecting the stroke-related fields on
		 * the paint. This mode can give unexpected results if the geometry
		 * is oriented counter-clockwise. This restriction does not apply to
		 * either FILL or STROKE.
		 */
		FILL_AND_STROKE(2);

		Style(int nativeInt) {
			this.nativeInt = nativeInt;
		}
		final int nativeInt;
	}

	public static class FontMetrics {
		/**
		 * The maximum distance above the baseline for the tallest glyph in
		 * the font at a given text size.
		 */
		public float top;
		/**
		 * The recommended distance above the baseline for singled spaced text.
		 */
		public float ascent;
		/**
		 * The recommended distance below the baseline for singled spaced text.
		 */
		public float descent;
		/**
		 * The maximum distance below the baseline for the lowest glyph in
		 * the font at a given text size.
		 */
		public float bottom;
		/**
		 * The recommended additional space to add between lines of text.
		 */
		public float leading;
	}

	public static class FontMetricsInt {
		public int top;
		public int ascent;
		public int descent;
		public int bottom;
		public int leading;

		@Override
		public String toString() {
			return "FontMetricsInt: top=" + top + " ascent=" + ascent +
			    " descent=" + descent + " bottom=" + bottom +
			    " leading=" + leading;
		}
	}

	public /*native*/ int getFlags() { return 0; }

	public /*native*/ int getHinting() { return 0; }
	public /*native*/ void setHinting(int mode) {}

	public /*native*/ void setDither(boolean dither) {}
	public /*native*/ void setLinearText(boolean linearText) {}
	public /*native*/ void setSubpixelText(boolean subpixelText) {}
	public /*native*/ void setUnderlineText(boolean underlineText) {}
	public /*native*/ void setStrikeThruText(boolean strikeThruText) {}
	public /*native*/ void setFakeBoldText(boolean fakeBoldText) {}

	public /*native*/ int getAlpha() { return 0; }
	public /*native*/ void setAlpha(int a) {}
	public /*native*/ float getStrokeWidth() { return 0; }

	public /*native*/ float getStrokeMiter() { return 0; }
	public /*native*/ void setStrokeMiter(float miter) {}
	public /*native*/ float getTextSize() { return 0; }

	public /*native*/ float getTextScaleX() { return 0; }
	public /*native*/ void setTextScaleX(float scaleX) {}
	public /*native*/ float getTextSkewX() { return 0; }
	public /*native*/ void setTextSkewX(float skewX) {}

	public /*native*/ float descent() { return 0; }
	public /*native*/ float getFontMetrics(FontMetrics metrics) { return 0; }
	public /*native*/ int getFontMetricsInt(FontMetricsInt fmi) { return 0; }

	public void setShadowLayer(float radius, float dx, float dy, int color) {}

	public Xfermode setXfermode(Xfermode xfermode) { return xfermode; }

	public enum Cap {
		/**
		 * The stroke ends with the path, and does not project beyond it.
		 */
		BUTT(0),
		/**
		 * The stroke projects out as a semicircle, with the center at the
		 * end of the path.
		 */
		ROUND(1),
		/**
		 * The stroke projects out as a square, with the center at the end
		 * of the path.
		 */
		SQUARE(2);

		private Cap(int nativeInt) {}
	}

	public enum Join {
		/**
		 * The outer edges of a join meet at a sharp angle
		 */
		MITER(0),
		/**
		 * The outer edges of a join meet in a circular arc.
		 */
		ROUND(1),
		/**
		 * The outer edges of a join meet with a straight line
		 */
		BEVEL(2);

		private Join(int nativeInt) {}
	}

	public void setStrokeCap(Cap cap) {}

	public void setStrokeJoin(Join join) {}

	public Typeface getTypeface() {
		return new Typeface();
	}

	private native long native_constructor();
	private native void native_set_color(long skia_paint, int color);
	private native int native_get_color(long skia_paint);
	private static native long native_create_font();
	private static native float native_ascent(long skia_font);
	private static native void native_set_typeface(long skia_font, long skia_typeface);
	private static native void native_set_text_size(long skia_font, float size);
	private static native float native_measure_text(long skia_font, CharSequence text, int start, int end, long skia_paint);
}
