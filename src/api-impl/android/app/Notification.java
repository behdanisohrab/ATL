package android.app;

import android.content.Context;
import android.graphics.Bitmap;
import android.media.AudioAttributes;
import android.media.session.MediaSession;
import android.net.Uri;
import android.os.Bundle;
import android.widget.RemoteViews;

public class Notification {

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

	public static class Builder {
		public Builder(Context context) {}

		public Builder setWhen(long when) {return this;}

		public Builder setSmallIcon(int icon, int level) {return this;}

		public Builder setContent(RemoteViews contentView) {return this;}

		public Builder setTicker(CharSequence tickerText, RemoteViews contentView) {return this;}

		public Builder setVibrate(long[] pattern) {return this;}

		public Builder setLights(int argb, int onMs, int offMs) {return this;}

		public Builder setOngoing(boolean ongoing) {return this;}

		public Builder setOnlyAlertOnce(boolean onlyAlertOnce) {return this;}

		public Builder setAutoCancel(boolean autoCancel) {return this;}

		public Builder setDefaults(int defaults) {return this;}

		public Builder setContentTitle(CharSequence title) {return this;}

		public Builder setContentText(CharSequence text) {return this;}

		public Builder setContentInfo(CharSequence info) {return this;}

		public Builder setContentIntent(PendingIntent intent) {return this;}

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

		public Builder addAction(Action action) {return this;}

		public Builder setStyle(Style style) {return this;}

		public Builder setExtras(Bundle extras) {return this;}

		public Notification build() {
			return new Notification();
		}
	}

	public static class Action {
		public static final class Builder {

			public Builder(int icon, CharSequence title, PendingIntent intent) {}

			public Builder addExtras(Bundle extras) {return this;}

			public Action build() {
				return new Action();
			}
		}
	}

	public static abstract class Style {}

	public static class MediaStyle extends Style {

		public MediaStyle setShowActionsInCompactView(int... viewActions) {return this;}

		public MediaStyle setMediaSession(MediaSession.Token token) {return this;}
	}
}
