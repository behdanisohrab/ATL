package android.widget;

import android.content.Context;
import android.util.AttributeSet;

public class SeekBar extends AbsSeekBar {

	public SeekBar(Context context) {
		super(context);
	}

	public SeekBar(Context context, AttributeSet attributeSet) {
		super(context, attributeSet);
	}

	public void setOnSeekBarChangeListener(OnSeekBarChangeListener l) {}

	public static interface OnSeekBarChangeListener {
	}

}
