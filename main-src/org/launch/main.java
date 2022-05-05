package org.launch;

import android.os.Looper;

public class main {
	public static void main(String[] args) {
		System.load("libnative/org_launch_main.so");
		real_main(args);
	}

	public static native void real_main(String[] args);
}
