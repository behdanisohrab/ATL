package android.app;

import android.content.Context;
import android.content.Intent;
import android.content.IntentSender;

public class PendingIntent {
	public static PendingIntent getBroadcast(Context context, int requestCode, Intent intent, int flags) {
		return new PendingIntent();
	}

	public IntentSender getIntentSender() {
		return null;
	}

	public void send(Context context, int code, Intent intent) {}

	public static PendingIntent getActivity(Context context, int requestCode, Intent intent, int flags) {
		return new PendingIntent();
	}

	public class CanceledException extends Exception {
	}
}
