package android.app;

import android.content.Context;
import android.content.ComponentName;
import android.content.Intent;
import android.os.Bundle;

import android.widget.TextView;
import android.view.View;
import android.view.LayoutInflater;
import android.view.Window;
import android.view.WindowManager;
import android.view.WindowManagerImpl;

import org.xmlpull.v1.XmlPullParser;
import org.xmlpull.v1.XmlPullParserFactory;

import java.io.FileReader;
import java.io.StringReader;

public class Activity extends Context {
	LayoutInflater layout_inflater;
	Application this_application;
	Window window = new Window();

	public Activity() {
		layout_inflater = new LayoutInflater();
		this_application = new Application(); // TODO: why is this different from the Activity Context?
	}

	public View root_view;

	public final Application getApplication () {
		return this_application;
	}

	public WindowManager getWindowManager() {
		return new WindowManagerImpl();
	}

	public ComponentName getComponentName() {
		return null;
	}

	public Intent getIntent() {
		return null; // this is the main activity, and it wasn't opened as a result of someone calling "open with"
//		return new Intent();
	}

	public boolean isFinishing() {
		return false; // FIXME
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

	public void onWindowFocusChanged(boolean hasFocus) {
		System.out.println("- onWindowFocusChanged - yay! (hasFocus: "+hasFocus+")");

		return;
	}

	/* --- */

    public void setContentView(int layoutResID) throws Exception {
		System.out.println("- setContentView - yay!");

		String layout_xml_file = "data/" + getString(layoutResID);

		System.out.println("loading layout from: " + layout_xml_file);

		XmlPullParserFactory factory = XmlPullParserFactory.newInstance();
		factory.setNamespaceAware(true);
		XmlPullParser xpp = factory.newPullParser();

		// cheating here, should look it up
		// note: s/@ref\///g
//		xpp.setInput( new StringReader ( "<?xml version=\"1.0\" encoding=\"utf-8\"?> <LinearLayout xmlns:android=\"http://schemas.android.com/apk/res/android\" android:orientation=\"1\" android:layout_width=\"-1\" android:layout_height=\"-1\">  <TextView android:id=\"0x7f030000\" android:layout_width=\"-2\" android:layout_height=\"-2\" android:text=\"Hello World!\" />  <TextView android:id=\"0x7f030001\" android:layout_width=\"-2\" android:layout_height=\"-2\" android:text=\"second one (static)\" /> </LinearLayout>" ) );
		xpp.setInput( new FileReader(layout_xml_file) );

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
		window.setContentView(view);
	}

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
		return this.window;
	}

    public final void runOnUiThread(Runnable action) {
        action.run(); // FIXME: running synchronously for now
    }
}
