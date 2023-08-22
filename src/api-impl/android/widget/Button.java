package android.widget;

import android.content.Context;
import android.content.res.TypedArray;
import android.util.AttributeSet;
import android.view.View;

public class Button extends View {

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

	public final void setText(CharSequence text) {
		native_setText(widget, String.valueOf(text));
	}

	@Override
	public void setOnClickListener(final OnClickListener l) {
		native_setOnClickListener(widget, l);
	}

}
