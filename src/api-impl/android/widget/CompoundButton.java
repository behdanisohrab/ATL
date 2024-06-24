package android.widget;

import android.content.Context;
import android.content.res.ColorStateList;
import android.graphics.drawable.Drawable;
import android.util.AttributeSet;

public abstract class CompoundButton extends Button implements Checkable {
	Drawable button_drawable = null;
	public Drawable mButtonDrawable;  // directly accessed by androidx

	public CompoundButton(Context context) {
		super(context);
	}

	public CompoundButton(Context context, AttributeSet attributeSet) {
		super(context, attributeSet);
	}

	@Override
	protected native long native_constructor(Context context, AttributeSet attrs);

	public static interface OnCheckedChangeListener {
		public void onCheckedChanged(CompoundButton buttonView, boolean isChecked);
	}

	public native void setOnCheckedChangeListener(OnCheckedChangeListener listener);

	@Override
	public native void setChecked(boolean checked);

	@Override
	public native boolean isChecked();

	public void toggle() {
		setChecked(!isChecked());
	}

	// following methods are overridden to prevent calling incompatible methods from superclasses
	@Override
	public void setOnClickListener(final OnClickListener l) {}
	@Override
	public void setText(CharSequence text) {}
	@Override
	public void setTextColor(int color) {}
	@Override
	public void setTextSize(float size) {}

	public void setButtonTintList(ColorStateList list) {
	}

	public void setButtonDrawable(Drawable drawable) {
		button_drawable = drawable;
	}

	public Drawable getButtonDrawable() {
		return button_drawable;
	}
}
