package android.media;

public class AudioTrack {
	public interface OnPlaybackPositionUpdateListener {
		void onMarkerReached(AudioTrack track);
		void onPeriodicNotification(AudioTrack track);
	}

	int streamType;
	int sampleRateInHz;
	int channelConfig;
	int audioFormat;
	int bufferSizeInBytes;
	int mode;
	private int sessionId;

	// for native code's use
	long pcm_handle;
	long params;
	int channels;
	int period_time;
	// mostly
	static int frames;
	OnPlaybackPositionUpdateListener periodic_update_listener;

	native void native_constructor(int streamType, int sampleRateInHz, int num_channels, int audioFormat, int bufferSizeInBytes, int mode);
	public AudioTrack(int streamType, int sampleRateInHz, int channelConfig, int audioFormat, int bufferSizeInBytes, int mode) {
		this.streamType = streamType;
		this.sampleRateInHz = sampleRateInHz;
		this.channelConfig = channelConfig;
		this.audioFormat = audioFormat;
		this.bufferSizeInBytes = bufferSizeInBytes;
		this.mode = mode;

		System.out.println("\n\n\nAudioTrack(" + streamType + ", " + sampleRateInHz + ", " + channelConfig + ", " + audioFormat + ", " + bufferSizeInBytes + ", " + mode + "); called\n\n\n\n");

		int num_channels;
		switch (channelConfig) {
			case 2:
				num_channels = 1;
				break;
			default:
				num_channels = 1;
		}

		native_constructor(streamType, sampleRateInHz, num_channels, audioFormat, bufferSizeInBytes, mode);
	}

	public AudioTrack(int streamType, int sampleRateInHz, int channelConfig, int audioFormat, int bufferSizeInBytes, int mode, int sessionId)  {
		this(streamType, sampleRateInHz, channelConfig, audioFormat, bufferSizeInBytes, mode);
		this.sessionId = sessionId;
	}

	public static native int getMinBufferSize(int sampleRateInHz, int channelConfig, int audioFormat);

	public void setPlaybackPositionUpdateListener(OnPlaybackPositionUpdateListener listener) {
		this.periodic_update_listener = listener;
	}

	public int setPositionNotificationPeriod(int periodInFrames) {
		System.out.println("\n\n\nsetPositionNotificationPeriod(" + periodInFrames + "); called\n\n\n\n");
		return 0; // SUCCESS
	}

	public int getPositionNotificationPeriod() {
		return this.frames;
	}

	public native void play();

	public void stop() {
		System.out.println("calling stop(), how did this not get reported before DIDREEEEEEEEEEEEEEEEEEEEEEEEE\n");
	}

	public void flush() {
		System.out.println("calling flush(), how did this not get reported before DIDREEEEEEEEEEEEEEEEEEEEEEEEE\n");
	}

	public void release() {
		System.out.println("calling release(), how did this not get reported before DIDREEEEEEEEEEEEEEEEEEEEEEEEE\n");
	}

	public int getState() {
		return 0; // TODO: fix up the native part and make this work properly
	}

	public native int write(byte[] audioData, int offsetInBytes, int sizeInBytes);

	public int getAudioSessionId() {
		return sessionId;
	}
}
