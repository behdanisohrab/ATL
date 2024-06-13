package android.widget;

import android.content.Context;
import android.util.AttributeSet;
import android.view.View;

public class ExpandableListView extends ListView {

	public ExpandableListView(Context context) {
		super(context);
	}

	public ExpandableListView(Context context, AttributeSet attributeSet) {
		super(context, attributeSet);
	}

	public static int getPackedPositionGroup(long packedPosition) {
		return 0;
	}

	public void setHeaderDividersEnabled(boolean enabled) {}

	public static interface OnChildClickListener {
		abstract boolean onChildClick(ExpandableListView parent, View v, int groupPosition, int childPosition, long id);
	}
}
