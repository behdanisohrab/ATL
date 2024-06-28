package android.widget;

import android.content.Context;
import android.graphics.drawable.Drawable;
import android.util.AttributeSet;
import android.view.View;

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

	public void addHeaderView(View v, Object data, boolean isSelectable) {}

	public void setDrawSelectorOnTop(boolean dummy) {}

	public void addHeaderView(View view) {}

	public boolean removeHeaderView(View view) { return true; }

	public int getHeaderViewsCount() {
		return 0;
	}

	public int getFooterViewsCount() {
		return 0;
	}

	public void setDivider(Drawable drawable) {}

	public void setSelectionFromTop(int position, int y) {}
}
