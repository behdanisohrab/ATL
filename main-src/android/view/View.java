package android.view;

import android.util.AttributeSet;

import org.gnome.gdk.Event;
import org.gnome.gtk.Gtk;
import org.gnome.gtk.Widget;
import org.gnome.gtk.Window;
import org.gnome.gtk.WindowPosition;
import org.gnome.gtk.Box;
import org.gnome.gtk.Container;
import org.gnome.gtk.Orientation;

import java.util.HashMap;

public class View extends Object { // FIXME-ish (it does extend Object, but in our case, we would want it to either extend or *be* a GObject implemented in C
	public int id;
	public ViewGroup parent;
	public AttributeSet attrs;

	public Widget widget;

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
