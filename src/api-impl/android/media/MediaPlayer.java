package android.media;

import android.content.Context;

public class MediaPlayer {
	public interface OnCompletionListener {
	}
	public interface OnErrorListener {
	}
	public interface OnPreparedListener {
	}
	public interface OnBufferingUpdateListener {
	}
	public interface OnInfoListener {
	}
	public interface OnSeekCompleteListener {
	}
	public interface OnVideoSizeChangedListener {
	}
	public interface MediaPlayerControl {
	}

	public static MediaPlayer create(Context context, int dummy) { return new MediaPlayer(); }
}
