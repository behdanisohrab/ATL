package org.launch;

import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;

import org.gnome.gdk.Event;
import org.gnome.gtk.Gtk;
import org.gnome.gtk.Widget;
import org.gnome.gtk.Window;
import org.gnome.gtk.WindowPosition;

import com.example.demo_application.MainActivity; // FIXME - get this dynamically (at least take it as a cmdline parameter)

class handle extends MainActivity {
	public void start() {
		super.onCreate(null); // FIXME - don't just use null
	}
}

public class main {
	public static void main(String[] args) {
        Gtk.init(args); // we will probably be calling Gtk functions soon

		handle activity_instance = new handle();

		activity_instance.start(); // this will run the onCreate method of the launch activity

        Gtk.main(); // main loop
	}
}
