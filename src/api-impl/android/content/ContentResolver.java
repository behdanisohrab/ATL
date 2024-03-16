package android.content;

import java.io.File;
import java.io.FileNotFoundException;

import android.database.ContentObserver;
import android.net.Uri;
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
		return ParcelFileDescriptor.open(new File(uri.uri), ParcelFileDescriptor.parseMode(mode));
	}
}
