package android.app;

import android.content.Context;
import android.content.Intent;
import android.os.IBinder;

public abstract class Service extends Context {

	public void onCreate() {
		System.out.println("Service.onCreate() called");
	}

	public abstract IBinder onBind(Intent intent);

	public abstract int onStartCommand(Intent intent, int flags, int startId);

	public void startForeground(int id, Notification notification) {
		System.out.println("startForeground(" + id + ", " + notification + ") called");
	}

	public void stopForeground(boolean remove) {
		System.out.println("stopForeground(" + remove + ") called");
	}

	public Application getApplication() {
		return this_application;
	}

	public void stopSelf(int startId) {
		System.out.println("Service.stopSelf(" + startId + ") called");
	}

	public void stopSelf() {
		System.out.println("Service.stopSelf() called");
	}
}
