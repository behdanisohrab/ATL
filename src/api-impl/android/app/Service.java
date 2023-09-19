package android.app;

import android.content.Context;
import android.content.Intent;
import android.os.IBinder;

public abstract class Service extends Context {

	public abstract void onCreate();

	public abstract IBinder onBind(Intent intent);

	public abstract int onStartCommand(Intent intent, int flags, int startId);

}
