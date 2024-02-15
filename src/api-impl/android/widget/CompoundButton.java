package android.widget;

import android.content.Context;
import android.util.AttributeSet;

public abstract class CompoundButton extends Button implements Checkable {

	public CompoundButton(Context context) {
		super(context);
	}
	
	public CompoundButton(Context context, AttributeSet attributeSet) {
		super(context, attributeSet);
	}

	@Override
	protected native long native_constructor(Context context, AttributeSet attrs);

	public static interface OnCheckedChangeListener {}

	public native void setOnCheckedChangeListener(OnCheckedChangeListener listener);

	public native void setChecked(boolean checked);

	public native boolean isChecked();

	// following methods are overridden to prevent calling incompatible methods from superclasses
	@Override
	public void setOnClickListener(final OnClickListener l) {}
	@Override
	public void setText(CharSequence text) {}
	@Override
	public void setTextColor(int color) {}
	@Override
	public void setTextSize(float size) {}
}
