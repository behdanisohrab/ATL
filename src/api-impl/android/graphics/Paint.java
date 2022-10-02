package android.graphics;

public class Paint {
	private int color = 0xFF000000;

    public void setColor(int color) {
		this.color = color;
	}

    public int getColor() {
		return color;
	}

	public void setAntiAlias(boolean aa) {}
	public void setStrokeWidth(float width) {}
    public void setTextSize(float textSize) {}

    public Typeface setTypeface(Typeface typeface) {
		return null;
	}
    public void getTextBounds(String text, int start, int end, Rect bounds) {}
    public void getTextBounds(char[] text, int index, int count, Rect bounds) {}
	public void setFlags(int flags) {}
	public void setFilterBitmap(boolean filter) {}
    public void setStyle(Style style) {}
    public float ascent() { return 0; }

    public float measureText(char[] text, int index, int count) { return 10; }
    public float measureText(String text, int start, int end) { return 10; }
    public float measureText(String text) { return 10; }
    public float measureText(CharSequence text, int start, int end) { return 10; }

    public enum Style {
        /**
         * Geometry and text drawn with this style will be filled, ignoring all
         * stroke-related settings in the paint.
         */
        FILL            (0),
        /**
         * Geometry and text drawn with this style will be stroked, respecting
         * the stroke-related fields on the paint.
         */
        STROKE          (1),
        /**
         * Geometry and text drawn with this style will be both filled and
         * stroked at the same time, respecting the stroke-related fields on
         * the paint. This mode can give unexpected results if the geometry
         * is oriented counter-clockwise. This restriction does not apply to
         * either FILL or STROKE.
         */
        FILL_AND_STROKE (2);
        
        Style(int nativeInt) {
            this.nativeInt = nativeInt;
        }
        final int nativeInt;
    }
}
