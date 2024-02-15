package android.widget;

import android.content.Context;
import android.util.AttributeSet;

public abstract class AbsListView extends AdapterView {

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
	public native void setOnItemSelectedListener(OnItemSelectedListener listener);

	@Override
	public native void setOnItemClickListener(OnItemClickListener listener);

	public native int getCheckedItemPosition();

	public interface OnScrollListener {}

}
