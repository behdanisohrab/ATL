package org.launch;

import android.os.Looper;

public class main {
	public static void main(String[] args) {
//		Looper.prepare(); // java.lang.RuntimeException: Can't create handler inside thread that has not called Looper.prepare()

		System.load("libnative/org_launch_main.so");
		real_main(args);
	}

	public static native void real_main(String[] args);
}
