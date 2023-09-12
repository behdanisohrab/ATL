package android.widget;

import android.content.Context;
import android.util.AttributeSet;

public class SeekBar extends ProgressBar {

	public SeekBar(Context context) {
		super(context);
	}

	public SeekBar(Context context, AttributeSet attributeSet) {
		super(context, attributeSet);
	}

	public interface OnSeekBarChangeListener {
	}

}
