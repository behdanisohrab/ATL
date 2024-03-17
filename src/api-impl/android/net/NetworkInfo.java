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

	private State state = State.DISCONNECTED;

	public NetworkInfo(boolean available) {
		state = available ? State.CONNECTED : State.DISCONNECTED;
	}

	public NetworkInfo.State getState() {
		return state;
	}

	public int getType() {
		return 0x8; // TYPE_DUMMY
	}

	public boolean isConnected() {
		return state == State.CONNECTED;
	}
}
