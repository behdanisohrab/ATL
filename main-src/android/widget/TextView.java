package android.widget;

import android.util.AttributeSet;
import android.content.Context;
import android.graphics.Typeface;
import android.content.res.ColorStateList;
import android.text.TextPaint;
import android.content.res.TypedArray;

import android.view.View;

public class TextView extends View {
	public String text;

	public TextView(int _id) { // FIXME
		id = _id;
	}

	public TextView(AttributeSet attrs) {
		super(attrs);

		native_constructor(attrs);
	}

	public TextView(Context context) {
		super(context);

		native_constructor(context);
	}

	native void native_constructor(AttributeSet attrs);
	native void native_constructor(Context context);

	public native final void setText(CharSequence _text);
    public native void setTextSize(float size);

    public void setTextColor(int color) {}
	public void setTextColor(ColorStateList colors) {}
    public void setTextSize(int unit, float size) {}
    public void setTypeface(Typeface tf, int style) {}
    public void setTypeface(Typeface tf) {}
    public void setLineSpacing(float add, float mult) {}
	public final void setLinksClickable(boolean whether) {}

	public final ColorStateList getTextColors() { return new ColorStateList(new int[][] { new int[0] }, new int[1]); }
	public static ColorStateList getTextColors(Context context, TypedArray attrs) { return new ColorStateList(new int[][] { new int[0] }, new int[1]); }

	public TextPaint getPaint() {
		return new TextPaint();
	}
}
