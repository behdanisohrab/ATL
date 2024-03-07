package android.widget;

import android.content.Context;
import android.graphics.drawable.Drawable;
import android.util.AttributeSet;

public class ListView extends AbsListView {

	public ListView(Context context) {
		super(context);
	}

	public ListView(Context context, AttributeSet attributeSet) {
		super(context, attributeSet);
	}

	public int getDividerHeight() {return 0;}

	public Drawable getDivider() {return null;}

	public void setTextFilterEnabled(boolean enabled) {}

}
