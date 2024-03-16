package android.net;

public class ConnectivityManager {

	public class NetworkCallback {}

	public NetworkInfo getNetworkInfo(int networkType) {
		return null; // this means the network type is not supported, which should make properly coded apps cease any attempts to use network-related APIs
	}

	public NetworkInfo getActiveNetworkInfo() {
		return new NetworkInfo();
	}

	public void registerNetworkCallback(NetworkRequest request, NetworkCallback callback) {}

	public boolean isActiveNetworkMetered() {
		return false;
	}
}
