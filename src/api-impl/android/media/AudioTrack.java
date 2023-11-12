package android.media;

public class AudioTrack {
	public interface OnPlaybackPositionUpdateListener {
		void onMarkerReached(AudioTrack track);
		void onPeriodicNotification(AudioTrack track);
	}

	public static final int PLAYSTATE_STOPPED = 1;
	public static final int PLAYSTATE_PAUSED  = 2;
	public static final int PLAYSTATE_PLAYING = 3;

	int streamType;
	int sampleRateInHz;
	int channelConfig;
	int audioFormat;
	int bufferSizeInBytes;
	int mode;
	private int sessionId;
	private int playbackState = PLAYSTATE_STOPPED;
	private int playbackHeadPosition = 0;

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
			case 12:
				num_channels = 2;
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

	public void play() {
		System.out.println("calling AudioTrack.play()\n");
		playbackState = PLAYSTATE_PLAYING;
		native_play();
	}

	public void stop() {
		System.out.println("calling stop(), how did this not get reported before DIDREEEEEEEEEEEEEEEEEEEEEEEEE\n");
		playbackState = PLAYSTATE_STOPPED;
	}

	public void flush() {
		System.out.println("calling flush(), how did this not get reported before DIDREEEEEEEEEEEEEEEEEEEEEEEEE\n");
	}

	public void release() {
		System.out.println("calling release(), how did this not get reported before DIDREEEEEEEEEEEEEEEEEEEEEEEEE\n");
	}

	public int getState() {
		return 1; // TODO: fix up the native part and make this work properly
	}

	public int write(byte[] audioData, int offsetInBytes, int sizeInBytes) {
		int framesToWrite = sizeInBytes / channels / 2;  // 2 means PCM16
		int ret = native_write(audioData, offsetInBytes, framesToWrite);
		if (ret > 0) {
			playbackHeadPosition += ret;
		}
		return ret * channels * 2; // 2 means PCM16
	}

	public int getAudioSessionId() {
		return sessionId;
	}

	public int getSampleRate() {
		return sampleRateInHz;
	}

	public int setStereoVolume(float leftVolume, float rightVolume) {
		return 0;
	}

	public int getPlayState() {
		return playbackState;
	}

	public void pause() {
		System.out.println("calling AudioTrack.pause()\n");
		playbackState = PLAYSTATE_PAUSED;
		native_pause();
	}

	public int getPlaybackHeadPosition() {
		return playbackHeadPosition;
	}

	public native void native_play();
	public native void native_pause();
	private native int native_write(byte[] audioData, int offsetInBytes, int sizeInBytes);
}
