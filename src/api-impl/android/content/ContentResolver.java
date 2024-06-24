package android.content;

import java.io.File;
import java.io.FileNotFoundException;

import android.database.AbstractCursor;
import android.database.ContentObserver;
import android.database.Cursor;
import android.net.Uri;
import android.os.CancellationSignal;
import android.os.ParcelFileDescriptor;

public class ContentResolver {
	public final void registerContentObserver(Uri uri, boolean notifyForDescendants, ContentObserver observer) {
	}
	public final void unregisterContentObserver(ContentObserver observer) {
	}
	public void notifyChange(Uri uri, ContentObserver observer) {
	}
	public int getUserId() {
		return 0;
	}
	public final void registerContentObserver(Uri uri, boolean notifyForDescendants, ContentObserver observer, int userHandle) {
	}

	public ParcelFileDescriptor openFileDescriptor(Uri uri, String mode) throws FileNotFoundException {
		return ParcelFileDescriptor.open(new File(uri.toString()), ParcelFileDescriptor.parseMode(mode));
	}

	public Cursor query(Uri uri, String[] projection, String selection, String[] selectionArgs, String sortOrder) {
		return new AbstractCursor() {
			public int getCount() { return 0; }
			public String[] getColumnNames() { return new String[0]; }
			public String getString(int column) { throw new IndexOutOfBoundsException(); }
			public short getShort(int column) { throw new IndexOutOfBoundsException(); }
			public int getInt(int column) { throw new IndexOutOfBoundsException(); }
			public long getLong(int column) { throw new IndexOutOfBoundsException(); }
			public float getFloat(int column) { throw new IndexOutOfBoundsException(); }
			public double getDouble(int column) { throw new IndexOutOfBoundsException(); }
			public boolean isNull(int column) { throw new IndexOutOfBoundsException(); }
		};
	}

	public Cursor query(Uri uri, String[] projection, String selection, String[] selectionArgs, String sortOrder, CancellationSignal cancellationSignal) {
		return query(uri, projection, selection, selectionArgs, sortOrder);
	}

	public int delete(Uri uri, String selection, String[] selectionArgs) {
		return 0;
	}
}
