package android.view;

import java.util.List;
import java.util.ArrayList;

public class InputDevice {

	public static final int SOURCE_CLASS_BUTTON = 0x00000001;
	public static final int SOURCE_CLASS_JOYSTICK = 0x00000010;
	public static final int SOURCE_CLASS_GAMEPAD = 0x00000400 | SOURCE_CLASS_BUTTON;
	public static final int SOURCE_KEYBOARD = 0x00000100 | SOURCE_CLASS_BUTTON;


	public static int[] getDeviceIds() {
		return new int[] {0}; // might work?
	}

	public static InputDevice getDevice(int id) {
		return new InputDevice();
	}

	public int getSources() {
		return 0; // FIXME
	}

	/*
	 * FIXME: We pretend we can do literally everything here...
	 */

	public boolean[] hasKeys(int... keys) {
		boolean[] ret = new boolean[keys.length];
		for (int i = 0; i < keys.length; i++) {
			ret[i] = true;
		}
		return ret;
	}

	public List<InputDevice.MotionRange>getMotionRanges() {
		MotionRange[] ranges = new MotionRange[32];

		for (int i = 0; i < ranges.length; i++) {
			ranges[i] = new MotionRange(i);
		}

		return new ArrayList<InputDevice.MotionRange>();
	}

	public class MotionRange {
		int axis;

		public MotionRange(int axis) {
			this.axis = axis;
		}

		public int getAxis() {
			return this.axis;
		}
	};
}
