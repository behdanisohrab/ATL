package android.widget;

import android.content.Context;
import android.util.AttributeSet;
import android.view.View;

public class Space extends View {

	public Space(Context context) {
		super(context);
		haveCustomMeasure = false;
	}

	public Space(Context context, AttributeSet attributeSet) {
		super(context, attributeSet);
		haveCustomMeasure = false;
	}

}