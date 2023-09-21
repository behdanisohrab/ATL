package android.os;

public interface Parcelable {
	public static interface Creator<T> {
		public T createFromParcel(Parcel parcel);
	}

	public static interface ClassLoaderCreator<T> extends Creator<T> {}
}
