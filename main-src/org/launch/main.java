package org.launch;

import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;

import com.example.demo_application.MainActivity; // FIXME

class handle extends MainActivity {
	void print_layout(View view, int indent) {

		for(int i = 0; i < indent; i++) {
			System.out.print("\t");
		}
		System.out.println(view);

		if(view instanceof ViewGroup) {
			for(View v : ((ViewGroup)view).children) {
				print_layout(v, indent + 1);
			}
		} else if(view instanceof TextView) {
			for(int i = 0; i < (indent + 1); i++) {
				System.out.print("\t");
			}
			System.out.println(((TextView)view).text);
		}

	}

	public void start() {
		super.onCreate(null); // FIXME - don't just use null

		System.out.println("-------------------------------------------------");
		System.out.println("-- this could *technically* be considered a UI --");
		System.out.println("-------------------------------------------------");
		print_layout(this.root_view, 0);
	}
}

public class main {
	public static void main(String[] a) {
		handle activity_instance = new handle();

		activity_instance.start();
	}
}
