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
	Window window = new Window();

	protected void set_window(long native_window) {
		window.native_window = native_window;
	}

	public Activity() {
		layout_inflater = new LayoutInflater();
	}

	public View root_view;

	public final Application getApplication () {
		return (Application)getApplicationContext();
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

	public final boolean requestWindowFeature(int featureId) {
		return false; // whatever feature it is, it's probably not supported
	}

	public final void setVolumeControlStream(int streamType) {}

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

		String layout_xml_file = android.os.Environment.getExternalStorageDirectory().getPath() + "/" + getString(layoutResID);

		System.out.println("loading layout from: " + layout_xml_file);

		XmlPullParserFactory factory = XmlPullParserFactory.newInstance();
		factory.setNamespaceAware(true);
		XmlPullParser xpp = factory.newPullParser();

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
