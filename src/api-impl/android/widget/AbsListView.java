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
	protected native void native_setAdapter(long widget, ListAdapter adapter);

	public void setChoiceMode(int choiceMode) {}

	public void setOnScrollListener(OnScrollListener onScrollListener) {}

	public void setAdapter(ListAdapter adapter) {
		super.setAdapter(adapter);
		native_setAdapter(this.widget, adapter);
	}

	public native void setItemChecked(int position, boolean value);

	@Override
	public native void setOnItemClickListener(OnItemClickListener listener);

	public native int getCheckedItemPosition();

	public void setCacheColorHint(int color) {}

	public int getListPaddingTop() {return 0;}

	public int getListPaddingBottom() {return 0;}

	public ListAdapter getAdapter() {
		return (ListAdapter) super.getAdapter();
	}

	public int pointToPosition(int x, int y) {
		return -1;
	}

	public interface OnScrollListener {}

	public interface SelectionBoundsAdjuster {}

}
