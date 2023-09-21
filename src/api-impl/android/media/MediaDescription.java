package android.media;

import android.graphics.Bitmap;
import android.net.Uri;
import android.os.Bundle;

public class MediaDescription {

	public static class Builder {

		public Builder setMediaId(String mediaId) {return this;}

		public Builder setTitle(CharSequence title) {return this;}

		public Builder setSubtitle(CharSequence subtitle) {return this;}

		public Builder setDescription(CharSequence description) {return this;}

		public Builder setIconBitmap(Bitmap iconBitmap) {return this;}

		public Builder setIconUri(Uri iconUri) {return this;}

		public Builder setExtras(Bundle extras) {return this;}

		public MediaDescription build() {
			return new MediaDescription();
		}
	}
	
}
