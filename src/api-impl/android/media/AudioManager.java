package android.media;

public class AudioManager {
	public static final String PROPERTY_OUTPUT_FRAMES_PER_BUFFER = "android.media.property.OUTPUT_FRAMES_PER_BUFFER";
	public static final String PROPERTY_OUTPUT_SAMPLE_RATE = "android.media.property.OUTPUT_SAMPLE_RATE";

	public static final int STREAM_MUSIC = 0x3;

	public boolean isBluetoothA2dpOn() {
		return false;
	}

	public String getProperty(String name) {
		switch (name) {
			case PROPERTY_OUTPUT_FRAMES_PER_BUFFER:
				return "256"; // FIXME arbitrary
			case PROPERTY_OUTPUT_SAMPLE_RATE:
				return "44100"; // FIXME arbitrary
			default:
				System.out.println("AudioManager.getProperty: >" + name + "< not handled");
				return "";
		}
	}

	public interface OnAudioFocusChangeListener {
	}

	public int getRingerMode() {
		return 0;
	}

	public int getStreamVolume(int streamType) {
		return 0; // arbitrary, shouldn't matter too much?
	}
}
