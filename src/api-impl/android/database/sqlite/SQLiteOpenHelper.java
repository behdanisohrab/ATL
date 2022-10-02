package android.database.sqlite;

import android.content.Context;
import android.database.sqlite.SQLiteDatabase.CursorFactory;

public abstract class SQLiteOpenHelper {
    public SQLiteOpenHelper(Context context, String name, CursorFactory factory, int version) {
        this(context, name, factory, version, null);
    }

    public SQLiteOpenHelper(Context context, String name, CursorFactory factory, int version, DatabaseErrorHandler errorHandler) {
        if (version < 1) throw new IllegalArgumentException("Version must be >= 1, was " + version);

/*        mContext = context;
        mName = name;
        mFactory = factory;
        mNewVersion = version;
        mErrorHandler = errorHandler;*/
    }

	public void close () {}

    public SQLiteDatabase getWritableDatabase() {
        synchronized (this) {
            return getDatabaseLocked(true);
        }
    }

    private SQLiteDatabase getDatabaseLocked(boolean writable) {
		return SQLiteDatabase.create(null); // return an empty database, surely the app can handle that
		/*
        if (mDatabase != null) {
            if (!mDatabase.isOpen()) {
                // Darn!  The user closed the database by calling mDatabase.close().
                mDatabase = null;
            } else if (!writable || !mDatabase.isReadOnly()) {
                // The database is already open for business.
                return mDatabase;
            }
        }

        if (mIsInitializing) {
            throw new IllegalStateException("getDatabase called recursively");
        }

        SQLiteDatabase db = mDatabase;
        try {
            mIsInitializing = true;

            if (db != null) {
                if (writable && db.isReadOnly()) {
                    db.reopenReadWrite();
                }
            } else if (mName == null) {
                db = SQLiteDatabase.create(null);
            } else {
                try {
                    if (DEBUG_STRICT_READONLY && !writable) {
                        final String path = mContext.getDatabasePath(mName).getPath();
                        db = SQLiteDatabase.openDatabase(path, mFactory,
                                SQLiteDatabase.OPEN_READONLY, mErrorHandler);
                    } else {
                        db = mContext.openOrCreateDatabase(mName, mEnableWriteAheadLogging ?
                                Context.MODE_ENABLE_WRITE_AHEAD_LOGGING : 0,
                                mFactory, mErrorHandler);
                    }
                } catch (SQLiteException ex) {
                    if (writable) {
                        throw ex;
                    }
                    Log.e(TAG, "Couldn't open " + mName
                            + " for writing (will try read-only):", ex);
                    final String path = mContext.getDatabasePath(mName).getPath();
                    db = SQLiteDatabase.openDatabase(path, mFactory,
                            SQLiteDatabase.OPEN_READONLY, mErrorHandler);
                }
            }

            onConfigure(db);

            final int version = db.getVersion();
            if (version != mNewVersion) {
                if (db.isReadOnly()) {
                    throw new SQLiteException("Can't upgrade read-only database from version " +
                            db.getVersion() + " to " + mNewVersion + ": " + mName);
                }

                db.beginTransaction();
                try {
                    if (version == 0) {
                        onCreate(db);
                    } else {
                        if (version > mNewVersion) {
                            onDowngrade(db, version, mNewVersion);
                        } else {
                            onUpgrade(db, version, mNewVersion);
                        }
                    }
                    db.setVersion(mNewVersion);
                    db.setTransactionSuccessful();
                } finally {
                    db.endTransaction();
                }
            }

            onOpen(db);

            if (db.isReadOnly()) {
                Log.w(TAG, "Opened " + mName + " in read-only mode");
            }

            mDatabase = db;
            return db;
        } finally {
            mIsInitializing = false;
            if (db != null && db != mDatabase) {
                db.close();
            }
        }
    */}
}
