package android.widget;

import android.content.Context;
import android.database.DataSetObserver;
import android.util.AttributeSet;

public abstract class AbsSpinner extends AdapterView {
	private Observer observer = new Observer();

	public AbsSpinner(Context context) {
		super(context);
		haveCustomMeasure = false;
	}

	public AbsSpinner(Context context, AttributeSet attributeSet) {
		super(context, attributeSet);
		haveCustomMeasure = false;
	}

	@Override
	protected native long native_constructor(Context context, AttributeSet attrs);
	protected native void native_setAdapter(long widget, SpinnerAdapter adapter);

	public void setAdapter(SpinnerAdapter adapter) {
		SpinnerAdapter oldAdapter = getAdapter();
		if (oldAdapter != null)
			oldAdapter.unregisterDataSetObserver(observer);
		super.setAdapter(adapter);
		if (adapter != null)
			adapter.registerDataSetObserver(observer);
		native_setAdapter(this.widget, adapter);
	}

	public SpinnerAdapter getAdapter() {
		return (SpinnerAdapter) super.getAdapter();
	}

	@Override
	public native void setOnItemSelectedListener(AdapterView.OnItemSelectedListener listener);

	private class Observer extends DataSetObserver {

		@Override
		public void onChanged() {
			AbsSpinner.this.native_setAdapter(widget, getAdapter());
		}
		@Override
		public void onInvalidated() {
			AbsSpinner.this.native_setAdapter(widget, getAdapter());
		}
	}
}