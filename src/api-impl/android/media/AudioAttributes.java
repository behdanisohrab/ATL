package android.media;

public class AudioAttributes {

	public int getFlags() {return 0;}

	public int getUsage() {return 0;}

	public class Builder {

		public Builder setContentType(int content_type) {
			return this;
		}

		public Builder setUsage(int usage) {
			return this;
		}

		public AudioAttributes build() {
			return new AudioAttributes();
		}
	}
}
