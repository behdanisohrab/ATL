package android.widget;

import android.content.Context;
import android.database.Cursor;

public abstract class CursorAdapter extends BaseAdapter {

	private Cursor cursor;

	public CursorAdapter(Context context, Cursor cursor, boolean autoRequery) {
		this.cursor = cursor;
	}

	public CursorAdapter(Context context, Cursor cursor, int flags) {
		this.cursor = cursor;
	}

	public void changeCursor(Cursor cursor) {
		this.cursor = cursor;
	}

	public Cursor getCursor() {
		return cursor;
	}

	@Override
	public int getCount() {
		return cursor == null ? 0 : cursor.getCount();
	}
}
