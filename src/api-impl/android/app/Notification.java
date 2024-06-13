package android.app;

import java.util.ArrayList;
import java.util.List;

import android.content.Context;
import android.graphics.Bitmap;
import android.media.AudioAttributes;
import android.media.session.MediaSession;
import android.net.Uri;
import android.os.Bundle;
import android.os.Parcelable;
import android.widget.RemoteViews;

public class Notification implements Parcelable {

	public static final AudioAttributes AUDIO_ATTRIBUTES_DEFAULT = new AudioAttributes();

	public long when;

	public int audioStreamType;

	public int icon;

	public PendingIntent deleteIntent;

	public CharSequence tickerText;

	public int iconLevel;

	public RemoteViews contentView;

	public long[] vibrate;

	public int ledARGB;

	public int ledOnMS;

	public int ledOffMS;

	public int flags;

	public int defaults;

	public Uri sound;

	public AudioAttributes audioAttributes;

	public Bundle extras;

	String text;
	String title;
	List<Action> actions = new ArrayList<Action>();
	PendingIntent intent;
	String iconPath;
	boolean ongoing;

	public String toString() {
		return "Notification [" + title + ", " + text + ", " + actions + "]";
	}

	public static class Builder {
		private Notification notification;

		public Builder(Context context) {
			notification = new Notification();
		}

		public Builder setWhen(long when) {return this;}

		public Builder setSmallIcon(int icon, int level) {
			notification.iconPath = Context.this_application.getString(icon);
			return this;
		}

		public Builder setContent(RemoteViews contentView) {return this;}

		public Builder setTicker(CharSequence tickerText, RemoteViews contentView) {return this;}

		public Builder setVibrate(long[] pattern) {return this;}

		public Builder setLights(int argb, int onMs, int offMs) {return this;}

		public Builder setOngoing(boolean ongoing) {
			notification.ongoing = ongoing;
			return this;
		}

		public Builder setOnlyAlertOnce(boolean onlyAlertOnce) {return this;}

		public Builder setAutoCancel(boolean autoCancel) {return this;}

		public Builder setDefaults(int defaults) {return this;}

		public Builder setContentTitle(CharSequence title) {
			notification.title = title != null ? title.toString() : null;
			return this;
		}

		public Builder setContentText(CharSequence text) {
			notification.text = text != null ? text.toString() : null;
			return this;
		}

		public Builder setContentInfo(CharSequence info) {return this;}

		public Builder setContentIntent(PendingIntent intent) {
			notification.intent = intent;
			return this;
		}

		public Builder setDeleteIntent(PendingIntent intent) {return this;}

		public Builder setFullScreenIntent(PendingIntent intent, boolean highPriority) {return this;}

		public Builder setLargeIcon(Bitmap icon) {return this;}

		public Builder setNumber(int number) {return this;}

		public Builder setProgress(int max, int progress, boolean indeterminate) {return this;}

		public Builder setSubText(CharSequence subText) {return this;}

		public Builder setUsesChronometer(boolean useChronometer) {return this;}

		public Builder setPriority(int priority) {return this;}

		public Builder setShowWhen(boolean showWhen) {return this;}

		public Builder setLocalOnly(boolean localOnly) {return this;}

		public Builder setGroup(String group) {return this;}

		public Builder setSortKey(String sortKey) {return this;}

		public Builder setGroupSummary(boolean isGroupSummary) {return this;}

		public Builder setCategory(String category) {return this;}

		public Builder setColor(int argb) {return this;}

		public Builder setVisibility(int visibility) {return this;}

		public Builder setPublicVersion(Notification notification) {return this;}

		public Builder setSound(Uri sound, AudioAttributes audioAttributes) {return this;}

		public Builder addAction(Action action) {
			notification.actions.add(action);
			return this;
		}

		public Builder setStyle(Style style) {
			if (style instanceof MediaStyle) {
				notification.ongoing = true;
			}
			return this;
		}

		public Builder setExtras(Bundle extras) {return this;}

		public Notification build() {
			return notification;
		}
	}

	public static class Action {

		int icon;
		String title;
		PendingIntent intent;

		public String toString() {
			return "Action [" + icon + ", " + title + ", " + intent + "]";
		}

		public static final class Builder {

			private Action action;
			public Builder(int icon, CharSequence title, PendingIntent intent) {
				action = new Action();
				action.icon = icon;
				action.title = String.valueOf(title);
				action.intent = intent;
			}

			public Builder addExtras(Bundle extras) {return this;}

			public Action build() {
				return action;
			}
		}
	}

	public static abstract class Style {}

	public static class MediaStyle extends Style {

		public MediaStyle setShowActionsInCompactView(int... viewActions) {return this;}

		public MediaStyle setMediaSession(MediaSession.Token token) {return this;}
	}

	public static class BigTextStyle extends Style {

		public BigTextStyle(Notification.Builder builder) {}

		public BigTextStyle setBigContentTitle(CharSequence title) {return this;}

		public BigTextStyle bigText(CharSequence text) {return this;}
	}
}
