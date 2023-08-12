package android.net;

public class Uri {
	public static Uri parse(String s) {
		return new Uri();
	}

	public Builder buildUpon() {
		return new Builder();
	}

	public static final class Builder {
		public Builder appendQueryParameter(String key, String value) {
			return this;
		}
	}
}
