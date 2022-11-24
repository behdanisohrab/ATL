package android.media;

public class AudioManager {
	public interface OnAudioFocusChangeListener {
	}

	public int getRingerMode() {
		return 0;
	}

	public int getStreamVolume (int streamType) {
		return 0; // arbitrary, shouldn't matter too much?
	}
}
