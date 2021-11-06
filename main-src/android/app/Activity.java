package android.app;

import android.content.Context;
import android.os.Bundle;

import android.widget.TextView;
import android.view.View;
import android.view.LayoutInflater;
import android.view.Window;

import org.xmlpull.v1.XmlPullParser;
import org.xmlpull.v1.XmlPullParserFactory;

import java.io.StringReader;

public class Activity extends Context {
	LayoutInflater layout_inflater;
	public View root_view;

	public Activity() {
		layout_inflater = new LayoutInflater();
	}

	protected void onCreate(Bundle savedInstanceState) {
		System.out.println("- onCreate - yay!");

		return;
	}

	protected void onStart() {
		System.out.println("- onStart - yay!");

		return;
	}

	protected void onRestart() {
		System.out.println("- onRestart - yay!");

		return;
	}

	protected void onResume() {
		System.out.println("- onResume - yay!");

		return;
	}

	protected void onPause() {
		System.out.println("- onPause - yay!");

		return;
	}

	protected void onStop() {
		System.out.println("- onStop - yay!");

		return;
	}

	protected void onDestroy() {
		System.out.println("- onDestroy - yay!");

		return;
	}

	/* --- */

    public void setContentView(int layoutResID) throws Exception {
		System.out.println("- setContentView - yay!");

		XmlPullParserFactory factory = XmlPullParserFactory.newInstance();
		factory.setNamespaceAware(true);
		XmlPullParser xpp = factory.newPullParser();

		// cheating here, should look it up
		// note: s/@ref\///g
//		xpp.setInput( new StringReader ( "<?xml version=\"1.0\" encoding=\"utf-8\"?> <LinearLayout xmlns:android=\"http://schemas.android.com/apk/res/android\" android:orientation=\"1\" android:layout_width=\"-1\" android:layout_height=\"-1\">  <TextView android:id=\"0x7f08010a\" android:layout_width=\"-2\" android:layout_height=\"-2\" android:text=\"Hello World!\" />  <TextView android:id=\"0x7f08010b\" android:layout_width=\"-2\" android:layout_height=\"-2\" android:text=\"second one (static)\" /> </LinearLayout>" ) );
		xpp.setInput( new StringReader ( "<RelativeLayout xmlns:android=\"http://schemas.android.com/apk/res/android\" xmlns:tools=\"http://schemas.android.com/tools\" android:layout_width=\"match_parent\" android:layout_height=\"match_parent\" android:paddingBottom=\"0\" android:paddingLeft=\"0\" android:paddingRight=\"0\" android:paddingTop=\"0\" tools:context=\".GDTRActivity\"> </RelativeLayout>" ) );

		root_view = layout_inflater.inflate(xpp, null, false);

		System.out.println("~~~~~~~~~~~");
		System.out.println(root_view);
		System.out.println(root_view.widget);
		System.out.println("~~~~~~~~~~~");

		setContentView(root_view);

/*		Window w = new Window();
		w.setTitle(this.toString());
		w.setDefaultSize(540, 960);
		w.add(root_view.widget);
		w.showAll();


        w.connect(new Window.DeleteEvent() {
            public boolean onDeleteEvent(Widget source, Event event) {
                Gtk.mainQuit();
                return false;
            }
        });*/
    }

	public void setContentView(View view) {
		set_widget_as_root(view.widget);
	}

	private native void set_widget_as_root(long widget);

	public <T extends android.view.View> T findViewById(int id) {
		System.out.println("- findViewById - asked for view with id: " + id);
		View view = View.view_by_id.get(id);
		System.out.println("- findViewById - found this: " + view);

		return (T) view;
	}

	public void invalidateOptionsMenu() {
		System.out.println("invalidateOptionsMenu() called, should we do something?");
	}

	public Window getWindow() {
		return new Window();
	}

    public final void runOnUiThread(Runnable action) {
        action.run(); // FIXME: running synchronously for now
    }
}
