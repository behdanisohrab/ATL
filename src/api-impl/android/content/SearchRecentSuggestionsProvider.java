package android.content;

import android.database.Cursor;
import android.net.Uri;
import android.os.ParcelFileDescriptor;

public class SearchRecentSuggestionsProvider extends ContentProvider {
	public void setupSuggestions(String s, int i) {}

	@Override
	public Cursor query(Uri uri, String[] projection, String selection, String[] selectionArgs, String sortOrder) {
		throw new UnsupportedOperationException("Unimplemented method 'query'");
	}

	@Override
	public Uri insert(Uri uri, ContentValues values) {
		throw new UnsupportedOperationException("Unimplemented method 'insert'");
	}

	@Override
	public int delete(Uri uri, String selection, String[] selectionArgs) {
		throw new UnsupportedOperationException("Unimplemented method 'delete'");
	}

	@Override
	public String getType(Uri uri) {
		throw new UnsupportedOperationException("Unimplemented method 'getType'");
	}

	@Override
	public ParcelFileDescriptor openFile(Uri uri, String mode) {
		throw new UnsupportedOperationException("Unimplemented method 'openFile'");
	}
}
