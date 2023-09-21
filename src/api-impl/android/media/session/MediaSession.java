package android.media.session;

import java.util.List;

import android.app.PendingIntent;
import android.content.Context;
import android.media.MediaDescription;
import android.media.MediaMetadata;
import android.os.Handler;

public class MediaSession {

	public static final class Token {}

	public static abstract class Callback {}

	public static class QueueItem {
		public QueueItem(MediaDescription description, long id) {}
	}

	public MediaSession(Context context, String tag) {}

	public Token getSessionToken() {
		return new Token();
	}

	public void setFlags(int flags) {}

	public void setCallback(Callback callback, Handler handler) {}

	public void setMediaButtonReceiver(PendingIntent pendingIntent)	{}

	public void setActive(boolean active) {}

	public void setPlaybackState(PlaybackState state) {}

	public void setMetadata(MediaMetadata metadata) {}

	public void setQueue(List<QueueItem> queue) {}
}
