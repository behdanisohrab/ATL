package android.widget;

import android.content.Context;
import android.util.AttributeSet;

public abstract class AbsListView extends AdapterView {

	public boolean mIsChildViewEnabled = false;  // this field gets directly accessed by androidx DropDownListView

	public AbsListView(Context context) {
		super(context);
	}

	public AbsListView(Context context, AttributeSet attributeSet) {
		super(context, attributeSet);
	}

	@Override
	protected native long native_constructor(Context context, AttributeSet attrs);

	public void setChoiceMode(int choiceMode) {}

	public void setOnScrollListener(OnScrollListener onScrollListener) {}

	public native void setAdapter(ListAdapter adapter);

	public native void setItemChecked(int position, boolean value);

	@Override
	public native void setOnItemClickListener(OnItemClickListener listener);

	public native int getCheckedItemPosition();

	public void setCacheColorHint(int color) {}

	public int getListPaddingTop() {return 0;}

	public int getListPaddingBottom() {return 0;}

	public int pointToPosition(int x, int y) {
		return -1;
	}

	public interface OnScrollListener {}

	public interface SelectionBoundsAdjuster {}

}
