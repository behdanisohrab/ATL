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

public class LinearLayout extends ViewGroup {

	Orientation orientation;

	public LinearLayout(AttributeSet _attrs) {
		attrs = _attrs;

		int _orientation = attrs.getAttributeIntValue("http://schemas.android.com/apk/res/android", "orientation", 0);

		if (_orientation == 0)
			orientation = Orientation.HORIZONTAL;
		else
			orientation = Orientation.VERTICAL;

		widget = (Container)(new Box(orientation, 1)); // spacing 1

		System.out.println("[[~~~~~~~~~~~");
		System.out.println(this);
		System.out.println(this.widget);
		System.out.println("[[~~~~~~~~~~~");

		System.out.println("creating LinearLayout with the attrs:"+attrs+"; "+attrs.getAttributeName(0));
	}
}
