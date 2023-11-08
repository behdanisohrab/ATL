package android.widget;

import android.content.Context;

public class Toast {

	private String text;

	public static Toast makeText(Context context, int resId, int duration) {
		return makeText(context, context.getString(resId), duration);
	}

	public static Toast makeText(Context context, CharSequence text, int duration) {
		Toast toast = new Toast();
		toast.text = String.valueOf(text);
		return toast;
	}

	public void show() {
		System.out.println("showing toast: " + text);
	}
}
