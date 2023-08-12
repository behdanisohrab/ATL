package android.view;

public class InputDevice {

	public static final int SOURCE_CLASS_BUTTON = 0x00000001;
	public static final int SOURCE_KEYBOARD = 0x00000100 | SOURCE_CLASS_BUTTON;

	public static int[] getDeviceIds() {
		return new int[] {0}; // might work?
	}

	public static InputDevice getDevice(int id) {
		return null;
	}
}
