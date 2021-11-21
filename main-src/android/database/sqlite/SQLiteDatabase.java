package android.database.sqlite;

import android.content.ContentValues;
import android.os.CancellationSignal;
import android.database.Cursor;

public final class SQLiteDatabase /*extends SQLiteClosable*/ {
// --- constants from android source
    /**
     * When a constraint violation occurs, an immediate ROLLBACK occurs,
     * thus ending the current transaction, and the command aborts with a
     * return code of SQLITE_CONSTRAINT. If no transaction is active
     * (other than the implied transaction that is created on every command)
     * then this algorithm works the same as ABORT.
     */
    public static final int CONFLICT_ROLLBACK = 1;

    /**
     * When a constraint violation occurs,no ROLLBACK is executed
     * so changes from prior commands within the same transaction
     * are preserved. This is the default behavior.
     */
    public static final int CONFLICT_ABORT = 2;

    /**
     * When a constraint violation occurs, the command aborts with a return
     * code SQLITE_CONSTRAINT. But any changes to the database that
     * the command made prior to encountering the constraint violation
     * are preserved and are not backed out.
     */
    public static final int CONFLICT_FAIL = 3;

    /**
     * When a constraint violation occurs, the one row that contains
     * the constraint violation is not inserted or changed.
     * But the command continues executing normally. Other rows before and
     * after the row that contained the constraint violation continue to be
     * inserted or updated normally. No error is returned.
     */
    public static final int CONFLICT_IGNORE = 4;

    /**
     * When a UNIQUE constraint violation occurs, the pre-existing rows that
     * are causing the constraint violation are removed prior to inserting
     * or updating the current row. Thus the insert or update always occurs.
     * The command continues executing normally. No error is returned.
     * If a NOT NULL constraint violation occurs, the NULL value is replaced
     * by the default value for that column. If the column has no default
     * value, then the ABORT algorithm is used. If a CHECK constraint
     * violation occurs then the IGNORE algorithm is used. When this conflict
     * resolution strategy deletes rows in order to satisfy a constraint,
     * it does not invoke delete triggers on those rows.
     * This behavior might change in a future release.
     */
    public static final int CONFLICT_REPLACE = 5;

    /**
     * Use the following when no conflict action is specified.
     */
    public static final int CONFLICT_NONE = 0;

    private static final String[] CONFLICT_VALUES = new String[]
            {"", " OR ROLLBACK ", " OR ABORT ", " OR FAIL ", " OR IGNORE ", " OR REPLACE "};

    /**
     * Maximum Length Of A LIKE Or GLOB Pattern
     * The pattern matching algorithm used in the default LIKE and GLOB implementation
     * of SQLite can exhibit O(N^2) performance (where N is the number of characters in
     * the pattern) for certain pathological cases. To avoid denial-of-service attacks
     * the length of the LIKE or GLOB pattern is limited to SQLITE_MAX_LIKE_PATTERN_LENGTH bytes.
     * The default value of this limit is 50000. A modern workstation can evaluate
     * even a pathological LIKE or GLOB pattern of 50000 bytes relatively quickly.
     * The denial of service problem only comes into play when the pattern length gets
     * into millions of bytes. Nevertheless, since most useful LIKE or GLOB patterns
     * are at most a few dozen bytes in length, paranoid application developers may
     * want to reduce this parameter to something in the range of a few hundred
     * if they know that external users are able to generate arbitrary patterns.
     */
    public static final int SQLITE_MAX_LIKE_PATTERN_LENGTH = 50000;

    /**
     * Open flag: Flag for {@link #openDatabase} to open the database for reading and writing.
     * If the disk is full, this may fail even before you actually write anything.
     *
     * {@more} Note that the value of this flag is 0, so it is the default.
     */
    public static final int OPEN_READWRITE = 0x00000000;          // update native code if changing

    /**
     * Open flag: Flag for {@link #openDatabase} to open the database for reading only.
     * This is the only reliable way to open a database if the disk may be full.
     */
    public static final int OPEN_READONLY = 0x00000001;           // update native code if changing

    private static final int OPEN_READ_MASK = 0x00000001;         // update native code if changing

    /**
     * Open flag: Flag for {@link #openDatabase} to open the database without support for
     * localized collators.
     *
     * {@more} This causes the collator <code>LOCALIZED</code> not to be created.
     * You must be consistent when using this flag to use the setting the database was
     * created with.  If this is set, {@link #setLocale} will do nothing.
     */
    public static final int NO_LOCALIZED_COLLATORS = 0x00000010;  // update native code if changing

    /**
     * Open flag: Flag for {@link #openDatabase} to create the database file if it does not
     * already exist.
     */
    public static final int CREATE_IF_NECESSARY = 0x10000000;     // update native code if changing

    /**
     * Open flag: Flag for {@link #openDatabase} to open the database file with
     * write-ahead logging enabled by default.  Using this flag is more efficient
     * than calling {@link #enableWriteAheadLogging}.
     *
     * Write-ahead logging cannot be used with read-only databases so the value of
     * this flag is ignored if the database is opened read-only.
     *
     * @see #enableWriteAheadLogging
     */
    public static final int ENABLE_WRITE_AHEAD_LOGGING = 0x20000000;

    /**
     * Absolute max value that can be set by {@link #setMaxSqlCacheSize(int)}.
     *
     * Each prepared-statement is between 1K - 6K, depending on the complexity of the
     * SQL statement & schema.  A large SQL cache may use a significant amount of memory.
     */
    public static final int MAX_SQL_CACHE_SIZE = 100;
// ---

    private SQLiteDatabase(String path, int openFlags, CursorFactory cursorFactory, DatabaseErrorHandler errorHandler) {
/*        mCursorFactory = cursorFactory;
        mErrorHandler = errorHandler != null ? errorHandler : new DefaultDatabaseErrorHandler();
        mConfigurationLocked = new SQLiteDatabaseConfiguration(path, openFlags);*/
    }

    public static SQLiteDatabase create(CursorFactory factory) {
        // This is a magic string with special meaning for SQLite.
        return openDatabase(":memory:", factory, CREATE_IF_NECESSARY);
    }

    private void open() {/*
        try {
            try {
                openInner();
            } catch (SQLiteDatabaseCorruptException ex) {
                onCorruption();
                openInner();
            }
        } catch (SQLiteException ex) {
            Log.e(TAG, "Failed to open database '" + getLabel() + "'.", ex);
            close();
            throw ex;
        }
    */}

    public long insert(String table, String nullColumnHack, ContentValues values) {
/*        try {
            return insertWithOnConflict(table, nullColumnHack, values, CONFLICT_NONE);
        } catch (SQLException e) {
            Log.e(TAG, "Error inserting " + values, e);
            return -1;
        }*/
		return -1;
    }

// ---
    public Cursor query(boolean distinct, String table, String[] columns,
            String selection, String[] selectionArgs, String groupBy,
            String having, String orderBy, String limit) {
        return queryWithFactory(null, distinct, table, columns, selection, selectionArgs,
                groupBy, having, orderBy, limit, null);
    }

    public Cursor query(boolean distinct, String table, String[] columns,
            String selection, String[] selectionArgs, String groupBy,
            String having, String orderBy, String limit, CancellationSignal cancellationSignal) {
        return queryWithFactory(null, distinct, table, columns, selection, selectionArgs,
                groupBy, having, orderBy, limit, cancellationSignal);
    }

    public Cursor query(String table, String[] columns, String selection,
            String[] selectionArgs, String groupBy, String having,
            String orderBy) {

        return query(false, table, columns, selection, selectionArgs, groupBy,
                having, orderBy, null /* limit */);
    }

    public Cursor query(String table, String[] columns, String selection,
            String[] selectionArgs, String groupBy, String having,
            String orderBy, String limit) {

        return query(false, table, columns, selection, selectionArgs, groupBy,
                having, orderBy, limit);
    }
// ---
    public Cursor queryWithFactory(CursorFactory cursorFactory,
            boolean distinct, String table, String[] columns,
            String selection, String[] selectionArgs, String groupBy,
            String having, String orderBy, String limit) {
        return queryWithFactory(cursorFactory, distinct, table, columns, selection,
                selectionArgs, groupBy, having, orderBy, limit, null);
    }

    public Cursor queryWithFactory(CursorFactory cursorFactory,
            boolean distinct, String table, String[] columns,
            String selection, String[] selectionArgs, String groupBy,
            String having, String orderBy, String limit, CancellationSignal cancellationSignal) {/*
        acquireReference();
        try {
            String sql = SQLiteQueryBuilder.buildQueryString(
                    distinct, table, columns, selection, groupBy, having, orderBy, limit);

            return rawQueryWithFactory(cursorFactory, sql, selectionArgs,
                    findEditTable(table), cancellationSignal);
        } finally {
            releaseReference();
        }*/
		return rawQueryWithFactory(cursorFactory, "XXX", selectionArgs, table, cancellationSignal);
    }
// ---
    public Cursor rawQueryWithFactory(
            CursorFactory cursorFactory, String sql, String[] selectionArgs,
            String editTable) {
        return rawQueryWithFactory(cursorFactory, sql, selectionArgs, editTable, null);
    }

    public Cursor rawQueryWithFactory(
            CursorFactory cursorFactory, String sql, String[] selectionArgs,
            String editTable, CancellationSignal cancellationSignal) {
//        acquireReference();
        try {
            SQLiteCursorDriver driver = new SQLiteDirectCursorDriver(this, sql, editTable,
                    cancellationSignal);
            return driver.query(cursorFactory/* != null ? cursorFactory : mCursorFactory*/,
                    selectionArgs);
        } finally {
//            releaseReference();
        }
    }
// ---
    public static SQLiteDatabase openDatabase(String path, CursorFactory factory, int flags) {
        return openDatabase(path, factory, flags, null);
    }

    public static SQLiteDatabase openDatabase(String path, CursorFactory factory, int flags, DatabaseErrorHandler errorHandler) {
        SQLiteDatabase db = new SQLiteDatabase(path, flags, factory, errorHandler);
        db.open();
        return db;
    }
// ---

    public interface CursorFactory {
        /**
         * See {@link SQLiteCursor#SQLiteCursor(SQLiteCursorDriver, String, SQLiteQuery)}.
         */
        public Cursor newCursor(SQLiteDatabase db,
                SQLiteCursorDriver masterQuery, String editTable,
                SQLiteQuery query);
    }
// ---

	public int update(String table, ContentValues values, String whereClause, String[] whereArgs) {
		return -1;
	}
}
