package android.hardware;

import android.os.Handler;

public class SensorManager {
    public Sensor getDefaultSensor(int type) {
		return null;
    }

	public boolean registerListener (SensorEventListener listener, Sensor sensor, int samplingPeriodUs, Handler handler) {
		return true; // we could try saying that the sensor doesn't exist and hope the app just doesn't use it then, but as long as we never call the handler the app should leave this alone
	}
}
