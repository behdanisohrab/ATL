package android.widget;

import android.content.Context;
import android.util.AttributeSet;
import android.view.ViewGroup;

public abstract class AdapterView extends ViewGroup {

	public AdapterView(Context context) {
		super(context);
	}

	public AdapterView(Context context, AttributeSet attributeSet) {
		super(context, attributeSet);
	}

	public interface OnItemSelectedListener {
	}

	public interface OnItemClickListener {
	}

	public void setAdapter(SpinnerAdapter adapter) {
	}

	public void setSelection(int i) {
	}

	public void setOnItemSelectedListener(AdapterView.OnItemSelectedListener listener) {}

	public void setOnItemClickListener(AdapterView.OnItemClickListener listener) {}

	public void setSelection(int position, boolean animate) {}
}
