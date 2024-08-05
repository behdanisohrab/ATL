package android.widget;

import android.content.Context;
import android.graphics.drawable.Drawable;
import android.util.AttributeSet;

public class AutoCompleteTextView extends EditText {

	public interface OnDismissListener {
	}

	public AutoCompleteTextView(Context context) {
		super(context);
	}

	public AutoCompleteTextView(Context context, AttributeSet attributeSet) {
		super(context, attributeSet);
	}

	public void setDropDownBackgroundDrawable(Drawable drawable) {}

	public int getThreshold() {
		return 0;
	}

	public void setOnItemClickListener(AdapterView.OnItemClickListener listener) {}

	public void setOnItemSelectedListener(AdapterView.OnItemSelectedListener listener) {}

	public int getDropDownAnchor() {
		return 0;
	}

	public void setAdapter(ListAdapter adapter) {}

	public void setThreshold(int threshold) {}

	public int getImeOptions() {
		return 0;
	}

}