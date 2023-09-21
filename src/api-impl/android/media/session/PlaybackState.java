package android.media.session;

public class PlaybackState {

	public class Builder {

		public Builder setState(int state, long position, float playbackSpeed, long updateTime) {return this;}

		public Builder setBufferedPosition(long bufferedPosition) {return this;}

		public Builder setActions(long actions) {return this;}

		public Builder setErrorMessage(CharSequence errorMessage) {return this;}

		public Builder setActiveQueueItemId(long activeQueueItemId) {return this;}

		public PlaybackState build() {
			return new PlaybackState();
		}

	}

}
