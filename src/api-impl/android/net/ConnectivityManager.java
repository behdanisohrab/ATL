package android.net;

class Network {}

public class ConnectivityManager {

	public class NetworkCallback {
		public void onAvailable(Network network) {}
		public void onLost(Network network) {}
	}

	public NetworkInfo getNetworkInfo(int networkType) {
		return null; // this means the network type is not supported, which should make properly coded apps cease any attempts to use network-related APIs
	}

	public NetworkInfo getActiveNetworkInfo() {
		return new NetworkInfo(nativeGetNetworkAvailable());
	}

	public native void registerNetworkCallback(NetworkRequest request, NetworkCallback callback);

	public native boolean isActiveNetworkMetered();

	protected native boolean nativeGetNetworkAvailable();

	public NetworkInfo[] getAllNetworkInfo() {
		return new NetworkInfo[] { getActiveNetworkInfo() };
	}
}
