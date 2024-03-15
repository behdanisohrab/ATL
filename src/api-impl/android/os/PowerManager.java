package android.os;

public final class PowerManager {
	public final class WakeLock {
		public void setReferenceCounted(boolean referenceCounted) {}

		public void acquire() {}

		public void release() {}
	}

	public WakeLock newWakeLock(int levelAndFlags, String tag) {
		return new WakeLock();
	}

	public void userActivity(long dummy, boolean dummy2) {}
}
