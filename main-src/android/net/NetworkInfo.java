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
}
