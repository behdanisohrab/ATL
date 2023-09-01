package android.widget;

import android.content.Context;
import android.util.AttributeSet;
import android.view.View;

public class Spinner extends View {

	public Spinner(Context context) {
		super(context);
	}

	public Spinner(Context context, AttributeSet attributeSet) {
		super(context, attributeSet);
	}

	public void setAdapter(SpinnerAdapter adapter) {
	}

	public void setSelection(int i) {
	}

	public void setOnItemSelectedListener(AdapterView.OnItemSelectedListener listener) {}

	public void setSelection(int position, boolean animate) {}

}