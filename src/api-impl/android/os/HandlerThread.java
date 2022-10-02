package android.os;

public class HandlerThread extends Thread {
	String name;

	public HandlerThread() {}

	public HandlerThread(String name) {
		this.name = name;
	}

	public void start() {
		// if(name.equals("FlurryAgent")) { return; }
	}

	public Looper getLooper() {
		return new Looper();
	}
}
