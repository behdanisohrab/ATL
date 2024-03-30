package android.widget;

import android.content.Context;
import android.content.res.TypedArray;
import android.graphics.drawable.Drawable;
import android.util.AttributeSet;

public class Button extends TextView {

	public Button(Context context) {
		super(context);
	}

	public Button(Context context, AttributeSet attributeSet) {
		super(context, attributeSet);

		TypedArray a = context.obtainStyledAttributes(attributeSet, com.android.internal.R.styleable.TextView, 0, 0);
		if (a.hasValue(com.android.internal.R.styleable.TextView_text)) {
			setText(a.getText(com.android.internal.R.styleable.TextView_text));
		}
		a.recycle();
	}

	@Override
	protected native long native_constructor(Context context, AttributeSet attrs);
	public native final void native_setText(long widget, String text);
	protected native void native_setOnClickListener(long widget, OnClickListener l);

	@Override
	public void setText(CharSequence text) {
		native_setText(widget, String.valueOf(text));
	}

	@Override
	public CharSequence getText() { return "FIXME Button.getText"; }

	@Override
	public void setTextSize(float size) {}

	@Override
	public void setOnClickListener(final OnClickListener l) {
		native_setOnClickListener(widget, l);
	}

	@Override
	public void setCompoundDrawables(Drawable left, Drawable top, Drawable right, Drawable bottom) {}

}
