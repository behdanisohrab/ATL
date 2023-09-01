package android.widget;

import android.content.Context;
import android.util.AttributeSet;
import android.view.View;

public class CheckBox extends View {

	public CheckBox(Context context) {
		super(context);
	}

	public CheckBox(Context context, AttributeSet attributeSet) {
		super(context, attributeSet);
	}

	public void setChecked(boolean checked) {}

	public void setOnCheckedChangeListener(CompoundButton.OnCheckedChangeListener listener) {}

	public boolean isChecked() {return false;}

}
