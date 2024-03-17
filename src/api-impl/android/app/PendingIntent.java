package android.app;

import android.content.Context;
import android.content.Intent;
import android.content.IntentSender;

public class PendingIntent {

	private int requestCode;
	Intent intent;
	int type;  // 0: activity, 1: service, 2: broadcast

	private PendingIntent(int requestCode, Intent intent, int type) {
		this.requestCode = requestCode;
		this.intent = intent;
		this.type = type;
	}
	public static PendingIntent getBroadcast(Context context, int requestCode, Intent intent, int flags) {
		return new PendingIntent(requestCode, intent, 2);
	}

	public IntentSender getIntentSender() {
		return null;
	}

	public void send(Context context, int code, Intent intent) {}

	public static PendingIntent getActivity(Context context, int requestCode, Intent intent, int flags) {
		return new PendingIntent(requestCode, intent, 0);
	}

	public static PendingIntent getService(Context context, int requestCode, Intent intent, int flags) {
		return new PendingIntent(requestCode, intent, 1);
	}

	public String toString() {
		return "PendingIntent [requestCode=" + requestCode + ", intent=" + intent + ", type="
			+ new String[] { "activity", "service", "broadcast" }[type] + "]";
	}

	public class CanceledException extends Exception {
	}
}
