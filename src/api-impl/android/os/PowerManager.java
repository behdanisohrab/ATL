package android.os;

public final class PowerManager {
	public final class WakeLock {}

	public WakeLock newWakeLock(int levelAndFlags, String tag) {
		return new WakeLock();
	}
}
