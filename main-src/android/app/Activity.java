package android.app;

import android.content.Context;
import android.os.Bundle;

import android.widget.TextView;
import android.view.View;
import android.view.LayoutInflater;

import org.xmlpull.v1.XmlPullParser;
import org.xmlpull.v1.XmlPullParserFactory;

import java.io.StringReader;

public class Activity extends Object { // we will want to extend something like _real_Activity implemented in C as a GObject
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
		xpp.setInput( new StringReader ( "<?xml version=\"1.0\" encoding=\"utf-8\"?> <LinearLayout xmlns:android=\"http://schemas.android.com/apk/res/android\" android:orientation=\"1\" android:layout_width=\"-1\" android:layout_height=\"-1\">  <TextView android:id=\"0x7f08010a\" android:layout_width=\"-2\" android:layout_height=\"-2\" android:text=\"Hello World!\" />  <TextView android:id=\"0x7f08010b\" android:layout_width=\"-2\" android:layout_height=\"-2\" android:text=\"second one (static)\" /> </LinearLayout>" ) );

		root_view = layout_inflater.inflate(xpp, null, false);

		return;
    }

	public <T extends android.view.View> T findViewById(int id) {
		System.out.println("- findViewById - asked for view with id: " + id);
		View view = View.view_by_id.get(id);
		System.out.println("- findViewById - found this: " + view);

		return (T) view;
	}
}
