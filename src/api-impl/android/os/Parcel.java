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

	public void writeByte(byte b) {
		System.out.println("Parcel.writeByte(" + b + ")");
	}

	public void writeString(String s) {
		System.out.println("Parcel.writeString(" + s + ")");
	}

	public void writeLong(long l) {
		System.out.println("Parcel.writeLong(" + l + ")");
	}

	public void writeInt(int i) {
		System.out.println("Parcel.writeInt(" + i + ")");
	}

	public byte[] marshall() {
		return new byte[0];
	}

	public int dataPosition() {
		return 0;
	}
}
