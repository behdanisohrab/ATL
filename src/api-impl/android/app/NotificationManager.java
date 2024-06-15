package android.app;

import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.os.Handler;
import android.os.Looper;

public class NotificationManager {
	public void cancelAll() {}

	public void notify(String tag, int id, Notification notification) {
		System.out.println("notify(" + tag + ", " + id + ", " + notification + ") called");
		long builder = nativeInitBuilder();
		for (Notification.Action action : notification.actions) {
			int intentType = -1;
			String actionName = null;
			String className = null;
			if (action.intent != null) {
				intentType = action.intent.type;
				actionName = action.intent.intent.getAction();
				className = action.intent.intent.getComponent() != null ? action.intent.intent.getComponent().getClassName() : null;
			}
			nativeAddAction(builder, action.title, intentType, actionName, className);
		}
		int intentType = -1;
		String actionName = null;
		String className = null;
		if (notification.intent != null) {
			intentType = notification.intent.type;
			actionName = notification.intent.intent.getAction();
			className = notification.intent.intent.getComponent() != null ? notification.intent.intent.getComponent().getClassName() : null;
		}
		nativeShowNotification(builder, id, notification.title, notification.text, notification.iconPath, notification.ongoing, intentType, actionName, className);
	}

	public void notify(int id, Notification notification) {
		notify(null, id, notification);
	}

	public void cancel(String tag, final int id) {
		// remove_notification doesn't work reliably when sent directly after add_notification in GNOME session.
		// So we give some extra delay here.
		new Handler(Looper.getMainLooper()).postDelayed(new Runnable() {
			@Override
			public void run() {
				nativeCancel(id);
			}
		}, 100);
	}

	public void cancel(int id) {
		cancel(null, id);
	}

	protected static void notificationActionCallback(int id, int intentType, String action, String className) {
		Context context = Context.this_application;
		Intent intent = new Intent(action);
		if (className != null && !className.isEmpty()) {
			intent.setComponent(new ComponentName(context, className));
		}
		if (intentType == 0) { // type Activity
			context.startActivity(intent);
		} else if (intentType == 1) { // type Service
			context.startService(intent);
		} else if (intentType == 2) { // type Broadcast
			context.sendBroadcast(intent);
		}
	}

	protected native long nativeInitBuilder();
	protected native void nativeAddAction(long builder, String title, int intentType, String action, String className);
	protected native void nativeShowNotification(long builder, int id, String title, String text, String iconPath, boolean ongoing, int intentType, String action, String className);
	protected native void nativeCancel(int id);
}
