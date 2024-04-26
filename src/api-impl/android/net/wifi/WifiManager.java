package android.net.wifi;

public class WifiManager {

	public class WifiLock {

		public void setReferenceCounted(boolean referenceCounted) {}

		public void release() {}

		public void acquire() {}

		public boolean isHeld() { return false; }
	}

	public WifiLock createWifiLock(int lockType, String tag) {
		return new WifiLock();
	}

	public WifiInfo getConnectionInfo() {
		return new WifiInfo();
	}
}
