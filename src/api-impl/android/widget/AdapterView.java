package android.widget;

import android.content.Context;
import android.util.AttributeSet;
import android.view.View;
import android.view.ViewGroup;

public abstract class AdapterView extends ViewGroup {

	private Adapter adapter;

	public AdapterView(Context context) {
		super(context);
	}

	public AdapterView(Context context, AttributeSet attributeSet) {
		super(context, attributeSet);
	}

	public AdapterView(Context context, AttributeSet attributeSet, int defStyle) {
		super(context, attributeSet, defStyle);
	}

	public interface OnItemSelectedListener {
	}

	public interface OnItemClickListener {
		public void onItemClick(AdapterView parent, View view, int position, long id);
	}

	public interface OnItemLongClickListener {
	}

	public void setAdapter(Adapter adapter) {
		this.adapter = adapter;
	}

	public void setSelection(int i) {
		setSelection(i, false);
	}

	public void setOnItemSelectedListener(AdapterView.OnItemSelectedListener listener) {}

	public void setOnItemClickListener(AdapterView.OnItemClickListener listener) {}

	public void setOnItemLongClickListener(AdapterView.OnItemLongClickListener listener) {}

	public void setSelection(int position, boolean animate) {}

	public Adapter getAdapter() {
		return adapter;
	}

	public Object getItemAtPosition(int position) {
		return adapter.getItem(position);
	}

	public void setEmptyView(View emptyView) {}

	public int getFirstVisiblePosition() {
		return 0;
	}

	public int getLastVisiblePosition() {
		return 0;
	}

	public int getCount() {
		return adapter.getCount();
	}
}
