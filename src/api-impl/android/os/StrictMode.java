package android.os;

public final class StrictMode {
	public static void setThreadPolicy(final ThreadPolicy policy) {}
	public static void setVmPolicy(final VmPolicy policy) {}
	public static ThreadPolicy allowThreadDiskWrites() {return null;}
	public static ThreadPolicy allowThreadDiskReads() {return null;}

	public static final class ThreadPolicy {
		public static final class Builder {
			public Builder detectAll() {
				return this;
			}
			public Builder permitDiskReads() {
				return this;
			}
			public Builder permitDiskWrites() {
				return this;
			}
			public Builder penaltyLog() {
				return this;
			}
			public Builder detectNetwork() {
				return this;
			}
			public ThreadPolicy build() {
				return new ThreadPolicy();
			}
		}
	}
	public static final class VmPolicy {
		public static final class Builder {
			public Builder detectActivityLeaks() {
				return this;
			}
			public Builder detectAll() {
				return this;
			}
			public Builder detectLeakedSqlLiteObjects() {
				return this;
			}
			public Builder detectLeakedClosableObjects() {
				return this;
			}
			public Builder detectLeakedRegistrationObjects() {
				return this;
			}
			public Builder detectFileUriExposure() {
				return this;
			}
			public Builder penaltyDeath() {
				return this;
			}
			public Builder penaltyLog() {
				return this;
			}
			public Builder penaltyDropBox() {
			return this;
			}
			public VmPolicy build() {
				return new VmPolicy();
			}
		}
	}
}
