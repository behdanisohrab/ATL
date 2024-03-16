package android.media;

import android.content.Context;
import java.io.FileDescriptor;

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

	public void setDataSource(FileDescriptor src, long dummy, long dummy2) {}
	public void setLooping(boolean dummy) {}
	public void setOnCompletionListener(MediaPlayer.OnCompletionListener dummy) {}
	public void setOnErrorListener(MediaPlayer.OnErrorListener dummy) {}
	public void setOnPreparedListener(MediaPlayer.OnPreparedListener dummy) {}
	public void setOnBufferingUpdateListener(MediaPlayer.OnBufferingUpdateListener dummy) {}
	public void setOnInfoListener(MediaPlayer.OnInfoListener dummy) {}
	public void setOnSeekCompleteListener(MediaPlayer.OnSeekCompleteListener dummy) {}
	public void setOnVideoSizeChangedListener(MediaPlayer.OnVideoSizeChangedListener dummy) {}
	public void setAudioStreamType(int dummy) {}
	public void start() {}
	public void pause() {}
	public void prepareAsync() {}
	public void reset() {}
	public void release() {}
}
