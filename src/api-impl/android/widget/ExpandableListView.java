package android.widget;

import android.content.Context;
import android.util.AttributeSet;

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
}
