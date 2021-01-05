package org.launch;

import com.example.android.basicimmersivemode.MainActivity;

class handle extends MainActivity {
	public void start() {
		super.onCreate(null); // fixme - don't just use null
	}
}

public class main {
	public static void main(String[] a) {
		handle activity_instance = new handle();

		activity_instance.start();
	}
}
