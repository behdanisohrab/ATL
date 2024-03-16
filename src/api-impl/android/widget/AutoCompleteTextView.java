package android.widget;

import android.content.Context;
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

}