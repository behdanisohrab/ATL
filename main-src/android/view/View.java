package android.view;

import android.util.AttributeSet;

import java.util.HashMap;

public class View extends Object { // FIXME-ish (it does extend Object, but in our case, we would want it to either extend or *be* a GObject implemented in C
	public int id;
	public ViewGroup parent;
	public AttributeSet attrs;

	public static HashMap<Integer, View> view_by_id = new HashMap<Integer, View>();

	public View() {} // FIXME

	public View(AttributeSet attrs) {
		id = attrs.getAttributeIntValue("http://schemas.android.com/apk/res/android", "id", 0);

		if(id != 0)
			view_by_id.put(id, this);
	}

	public void setLayoutParams(ViewGroup.LayoutParams params) {
		if (params == null) {
			throw new NullPointerException("Layout parameters cannot be null");
		}
		System.out.println("would set layout params on view: "+id+" ("+params+")");
		/*mLayoutParams = params;
		resolveLayoutParams();
		if (mParent instanceof ViewGroup) {
			((ViewGroup) mParent).onSetLayoutParams(this, params);
		}
		requestLayout();*/
	}

	protected void onFinishInflate() {
	}
}
