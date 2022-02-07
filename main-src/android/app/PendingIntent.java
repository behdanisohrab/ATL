package android.app;

import android.content.Context;
import android.content.Intent;

public class PendingIntent {
	public static PendingIntent getBroadcast(Context context, int requestCode, Intent intent, int flags) {
		return new PendingIntent();
	}
}
