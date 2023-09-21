package android.os;

public class Parcel {

	public static Parcel obtain() {
		return new Parcel();
	}

	public void writeBundle(Bundle bundle) {}

	public Bundle readBundle(ClassLoader loader) {
		return new Bundle();
	}

	public void setDataPosition(int position) {}

	public void recycle() {}
}
