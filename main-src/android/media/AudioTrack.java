package android.media;

public class AudioTrack {
	public interface OnPlaybackPositionUpdateListener {
	}

	int streamType;
	int sampleRateInHz;
	int channelConfig;
	int audioFormat;
	int bufferSizeInBytes;
	int mode;

	public AudioTrack (int streamType, int sampleRateInHz, int channelConfig, int audioFormat, int bufferSizeInBytes, int mode) {
		this.streamType = streamType;
		this.sampleRateInHz = sampleRateInHz;
		this.channelConfig = channelConfig;
		this.audioFormat = audioFormat;
		this.bufferSizeInBytes = bufferSizeInBytes;
		this.mode = mode;
	}
}
