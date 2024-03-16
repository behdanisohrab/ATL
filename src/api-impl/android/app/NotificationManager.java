package android.app;

public class NotificationManager {
	public void cancelAll() {}

	public void notify(String tag, int id, Notification notification) {
		System.out.println("notify(" + tag + ", " + id + ", " + notification + ") called");
	}

	public void notify(int id, Notification notification) {
		System.out.println("notify(" + id + ", " + notification + ") called");
	}
}
