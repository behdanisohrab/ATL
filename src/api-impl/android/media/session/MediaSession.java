package android.media.session;

import java.util.List;

import android.app.PendingIntent;
import android.content.Context;
import android.media.MediaDescription;
import android.media.MediaMetadata;
import android.os.Handler;

public class MediaSession {

	private List<QueueItem> queue;

	public static final class Token {}

	public static abstract class Callback {}

	public static class QueueItem {
		long id;
		MediaDescription description;

		public QueueItem(MediaDescription description, long id) {
			this.description = description;
			this.id = id;
		}
	}

	public MediaSession(Context context, String tag) {}

	public Token getSessionToken() {
		return new Token();
	}

	public void setFlags(int flags) {}

	public void setCallback(Callback callback, Handler handler) {
		nativeSetCallback(callback);
	}

	public void setCallback(Callback callback) {
		nativeSetCallback(callback);
	}

	public void setMediaButtonReceiver(PendingIntent pendingIntent)	{}

	public void setActive(boolean active) {}

	public void setPlaybackState(PlaybackState state) {
		String title = null;
		String subTitle = null;
		String artUrl = null;
		if (queue != null) for (QueueItem item : queue) {
			if (item.id == state.activeQueueItemId) {
				title = item.description.title.toString();
				subTitle = item.description.subtitle.toString();
				artUrl = item.description.iconUri.toString();
				break;
			}
		}
		nativeSetState(state.state, state.actions, state.position, state.updateTime, title, subTitle, artUrl);
	}

	public void setMetadata(MediaMetadata metadata) {}

	public void setQueue(List<QueueItem> queue) {
		this.queue = queue;
	}

	public void release() {}

	protected native void nativeSetState(int state, long actions, long position, long updateTime, String title, String subTitle, String artUrl);
	protected native void nativeSetCallback(Callback callback);
}
