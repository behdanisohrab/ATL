package android.app;

public class AlarmManager {
	public void cancel(PendingIntent operation) {}

	public void setInexactRepeating(int type, long triggerTime, long interval, PendingIntent operation) {}

	public void setExact(int type, long triggerTime, PendingIntent operation) {}
}
