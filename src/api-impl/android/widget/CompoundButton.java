package android.widget;

import android.content.Context;
import android.util.AttributeSet;

public abstract class CompoundButton extends Button {
	
	public CompoundButton(Context context) {
		super(context);
	}
	
	public CompoundButton(Context context, AttributeSet attributeSet) {
		super(context, attributeSet);
	}

	public static interface OnCheckedChangeListener {}

	public void setOnCheckedChangeListener(OnCheckedChangeListener listener) {}
}
