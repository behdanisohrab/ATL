package android.content;

import java.io.File;
import java.io.FileNotFoundException;

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
		if ("file".equals(uri.getScheme())) {
			return ParcelFileDescriptor.open(new File(uri.toString()), ParcelFileDescriptor.parseMode(mode));
		} else {
			ContentProvider provider = ContentProvider.providers.get(uri.getAuthority());
			if (provider != null)
				return provider.openFile(uri, mode);
			else
				return null;
		}
	}

	public Cursor query(Uri uri, String[] projection, String selection, String[] selectionArgs, String sortOrder) {
		ContentProvider provider = ContentProvider.providers.get(uri.getAuthority());
		if (provider != null)
			return provider.query(uri, projection, selection, selectionArgs, sortOrder);
		else
			return null;
	}

	public Cursor query(Uri uri, String[] projection, String selection, String[] selectionArgs, String sortOrder, CancellationSignal cancellationSignal) {
		return query(uri, projection, selection, selectionArgs, sortOrder);
	}

	public int delete(Uri uri, String selection, String[] selectionArgs) {
		ContentProvider provider = ContentProvider.providers.get(uri.getAuthority());
		if (provider != null)
			return provider.delete(uri, selection, selectionArgs);
		else
			return 0;
	}

	public Uri insert(Uri uri, ContentValues values) {
		ContentProvider provider = ContentProvider.providers.get(uri.getAuthority());
		if (provider != null)
			return provider.insert(uri, values);
		else
			return null;
	}

	public String getType(Uri uri) {
		ContentProvider provider = ContentProvider.providers.get(uri.getAuthority());
		if (provider != null)
			return provider.getType(uri);
		else
			return null;
	}
}
