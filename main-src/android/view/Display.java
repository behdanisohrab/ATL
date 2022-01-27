package android.view;

import android.util.DisplayMetrics;

public final class Display {
	// FIXME: what do we return here?
	// we don't want to hardcode this stuff, but at the same time the apps can cache it
	public void getMetrics(DisplayMetrics outMetrics) {
		outMetrics.widthPixels = 960;
		outMetrics.heightPixels = 540;
	}

	public int getRotation() {
		return 0/*ROTATION_0*/;
	}
}
