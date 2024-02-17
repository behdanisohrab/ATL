package android.widget;

import android.content.Context;
import android.util.AttributeSet;

public class Spinner extends AbsSpinner {

	public Spinner(Context context) {
		super(context);
	}

	public Spinner(Context context, AttributeSet attributeSet) {
		super(context, attributeSet);
	}

	public void setAdapter(SpinnerAdapter adapter) {
		super.setAdapter(adapter);
	}

}