package android.net;

public class ConnectivityManager {
	public NetworkInfo getNetworkInfo (int networkType) {
		return null; // this means the network type is not supported, which should make properly coded apps cease any attempts to use network-related APIs
	}
}
