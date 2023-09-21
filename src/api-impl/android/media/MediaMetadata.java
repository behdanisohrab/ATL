package android.media;

import android.os.Parcel;
import android.os.Parcelable.Creator;

public class MediaMetadata {

	public static final Creator<MediaMetadata> CREATOR = new Creator<MediaMetadata>() {
		@Override
		public MediaMetadata createFromParcel(Parcel source) {
			return new MediaMetadata();
		}
	};

	public void writeToParcel(Parcel dest, int flags) {}
}
