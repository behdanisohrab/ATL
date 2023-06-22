package android.widget;

import android.content.Context;
import android.util.AttributeSet;
import android.view.View;

public class ProgressBar extends View {
	public ProgressBar(AttributeSet attrs) {
		super(attrs);

		native_constructor(attrs);
	}

	public ProgressBar(Context context) {
		super(context);

		//		native_constructor(context);
	}

	private native void native_constructor(AttributeSet attrs);
	private native void native_constructor(Context context);

	public synchronized void setIndeterminate(boolean indeterminate) {}
}
