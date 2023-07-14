package android.location;

import java.util.HashSet;
import java.util.Set;

public class LocationManager {

	static Set<LocationListener> listeners = new HashSet<>();

	public String getBestProvider(Criteria criteria, boolean enabledOnly) {
		return "xdgportal";
	}

	public Location getLastKnownLocation(String provider) {
		return null;
	}

	public void requestLocationUpdates (String provider, long minTimeMs, float minDistanceM, LocationListener listener) {
		listeners.add(listener);
		nativeGetLocation();
	}

	private native void nativeGetLocation();

	private static void locationUpdated(double latitude, double longitude, double heading) {
		for (LocationListener locationListener : listeners) {
			locationListener.onLocationChanged(new Location(latitude, longitude, heading));
		}
	}
	
}
