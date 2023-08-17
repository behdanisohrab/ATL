package android.widget;

import android.content.Context;
import android.util.AttributeSet;
import android.view.View;

public class ProgressBar extends View {
	public ProgressBar(Context context, AttributeSet attrs) {
		super(context, attrs);
	}

	public ProgressBar(Context context) {
		super(context);
	}

	public synchronized void setIndeterminate(boolean indeterminate) {}
}
