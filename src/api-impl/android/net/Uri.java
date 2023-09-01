package android.net;

import java.net.URI;

public class Uri {
	private URI uri;

	public static Uri parse(String s) {
		Uri ret = new Uri();
		try {
			ret.uri = URI.create(s);
		} catch (IllegalArgumentException e) {
		}
		return ret;
	}

	public Builder buildUpon() {
		return new Builder();
	}

	public static final class Builder {
		public Builder appendQueryParameter(String key, String value) {
			return this;
		}
	}

	@Override
	public String toString() {
		return String.valueOf(uri);
	}
}
