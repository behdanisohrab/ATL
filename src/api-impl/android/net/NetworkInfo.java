package android.net;

public class NetworkInfo {
	public enum State {
		CONNECTED,
		CONNECTING,
		DISCONNECTED,
		DISCONNECTING,
		SUSPENDED,
		UNKNOWN
	}

	public NetworkInfo.State getState() {
		return State.DISCONNECTED;
	}

	public int getType() {
		return 0x8; // where did you even get a NetworkInfo object... there is no network
	}

	public boolean isConnected() {
		return true;
	}
}
