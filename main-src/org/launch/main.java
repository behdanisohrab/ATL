package org.launch;

import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;

import com.example.demo_application.MainActivity; // FIXME - get this dynamically (at least take it as a cmdline parameter)

public class main {
	public static void main(String[] args) {
		System.load("/home/Mis012/Github_and_other_sources/android_translation_layer_PoC/libnative/org_launch_main.so");
		real_main(args);
	}

	public static native void real_main(String[] args);
}
