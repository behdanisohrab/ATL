package android.view;

import android.util.AttributeSet;

import org.gnome.gdk.Event;
import org.gnome.gtk.Gtk;
import org.gnome.gtk.Widget;
import org.gnome.gtk.Window;
import org.gnome.gtk.WindowPosition;
import org.gnome.gtk.Container;

import java.util.ArrayList;

public class ViewGroup extends View {
	public int id;
	public ArrayList<View> children;

	public ViewGroup() {
		children = new ArrayList<View>();
	}

	public ViewGroup(int _id) { // FIXME
		children = new ArrayList<View>();

		id = _id;
	}

	public void addView(View child, LayoutParams params) {
		addView(child, -1, params);
	}

	public void addView(View child, int index, LayoutParams params) {
		System.out.println("adding view `"+child+"` to parent `"+this+"`");

		child.parent = this;
		children.add(child);
		((Container)widget).add(child.widget);
	}

	public LayoutParams generateLayoutParams(AttributeSet attrs) {
		return new LayoutParams(/*getContext(), attrs*/);
	}

	public static class LayoutParams {
		public static final int FILL_PARENT = -1;
		public static final int MATCH_PARENT = -1;
		public static final int WRAP_CONTENT = -2;

		public int width;

		public int height;

		/**
		 * Used to animate layouts.
		 */
//		public LayoutAnimationController.AnimationParameters layoutAnimationParameters;
	}
}
