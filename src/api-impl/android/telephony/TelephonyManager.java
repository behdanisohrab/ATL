package android.telephony;

public class TelephonyManager {
	public String getNetworkOperatorName() {
		return "none"; // FIXME: can we return null here?
	}

	public int getPhoneType() {
		return 0; // PHONE_TYPE_NONE
	}
}
