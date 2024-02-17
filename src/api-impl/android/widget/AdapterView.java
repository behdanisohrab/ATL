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

	public interface OnItemSelectedListener {
	}

	public interface OnItemClickListener {
		public void onItemClick(AdapterView parent, View view, int position, long id);
	}

	public void setAdapter(Adapter adapter) {
		this.adapter = adapter;
	}

	public void setSelection(int i) {
	}

	public void setOnItemSelectedListener(AdapterView.OnItemSelectedListener listener) {}

	public void setOnItemClickListener(AdapterView.OnItemClickListener listener) {}

	public void setSelection(int position, boolean animate) {}

	public Adapter getAdapter() {
		return adapter;
	}
}
