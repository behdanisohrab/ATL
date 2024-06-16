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

	public boolean isConnectedOrConnecting() {
		return false;
	}

	public int getSubtype() {
		return 0;  // NETWORK_TYPE_UNKNOWN
	}

	public boolean isRoaming() {
		return false;
	}

	public String getTypeName() {
		return "UNKNOWN";
	}

	public String getSubtypeName() {
		return "UNKNOWN";
	}
}
