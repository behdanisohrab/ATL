/*
 * Copyright (C) 2007 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
/*
** Modified to support SQLite extensions by the SQLite developers: 
** sqlite-dev@sqlite.org.
*/
/*
** Rewritten from C++ to C for Android Translation Layer: 
*/

#include <stdio.h>
#include <stdlib.h>

#include "android_database_SQLiteCommon.h"

/* throw a SQLiteException with a message appropriate for the error in handle */
void throw_sqlite3_exception_handle(JNIEnv* env, sqlite3* handle) {
	throw_sqlite3_exception_handle_message(env, handle, NULL);
}

/* throw a SQLiteException with the given message */
void throw_sqlite3_exception_message(JNIEnv* env, const char* message) {
	throw_sqlite3_exception_handle_message(env, NULL, message);
}

/* throw a SQLiteException with a message appropriate for the error in handle
   concatenated with the given message
 */
void throw_sqlite3_exception_handle_message(JNIEnv* env, sqlite3* handle, const char* message) {
	if (handle) {
	// get the error code and message from the SQLite connection
	// the error message may contain more information than the error code
	// because it is based on the extended error code rather than the simplified
	// error code that SQLite normally returns.
	throw_sqlite3_exception_errcode_message(env, sqlite3_extended_errcode(handle),
		sqlite3_errmsg(handle), message);
	} else {
	// we use SQLITE_OK so that a generic SQLiteException is thrown;
	// any code not specified in the switch statement below would do.
	throw_sqlite3_exception_errcode_message(env, SQLITE_OK, "unknown error", message);
	}
}

/* throw a SQLiteException for a given error code
 * should only be used when the database connection is not available because the
 * error information will not be quite as rich */
void throw_sqlite3_exception_errcode(JNIEnv* env, int errcode, const char* message) {
	throw_sqlite3_exception_errcode_message(env, errcode, "unknown error", message);
}

/* throw a SQLiteException for a given error code, sqlite3message, and
   user message
 */
void throw_sqlite3_exception_errcode_message(JNIEnv* env, int errcode,
				 const char* sqlite3Message, const char* message) {
	const char* exceptionClass;
	switch (errcode & 0xff) { /* mask off extended error code */
	case SQLITE_IOERR:
		exceptionClass = "android/database/sqlite/SQLiteDiskIOException";
		break;
	case SQLITE_CORRUPT:
	case SQLITE_NOTADB: // treat "unsupported file format" error as corruption also
		exceptionClass = "android/database/sqlite/SQLiteDatabaseCorruptException";
		break;
	case SQLITE_CONSTRAINT:
		exceptionClass = "android/database/sqlite/SQLiteConstraintException";
		break;
	case SQLITE_ABORT:
		exceptionClass = "android/database/sqlite/SQLiteAbortException";
		break;
	case SQLITE_DONE:
		exceptionClass = "android/database/sqlite/SQLiteDoneException";
		sqlite3Message = NULL; // SQLite error message is irrelevant in this case
		break;
	case SQLITE_FULL:
		exceptionClass = "android/database/sqlite/SQLiteFullException";
		break;
	case SQLITE_MISUSE:
		exceptionClass = "android/database/sqlite/SQLiteMisuseException";
		break;
	case SQLITE_PERM:
		exceptionClass = "android/database/sqlite/SQLiteAccessPermException";
		break;
	case SQLITE_BUSY:
		exceptionClass = "android/database/sqlite/SQLiteDatabaseLockedException";
		break;
	case SQLITE_LOCKED:
		exceptionClass = "android/database/sqlite/SQLiteTableLockedException";
		break;
	case SQLITE_READONLY:
		exceptionClass = "android/database/sqlite/SQLiteReadOnlyDatabaseException";
		break;
	case SQLITE_CANTOPEN:
		exceptionClass = "android/database/sqlite/SQLiteCantOpenDatabaseException";
		break;
	case SQLITE_TOOBIG:
		exceptionClass = "android/database/sqlite/SQLiteBlobTooBigException";
		break;
	case SQLITE_RANGE:
		exceptionClass = "android/database/sqlite/SQLiteBindOrColumnIndexOutOfRangeException";
		break;
	case SQLITE_NOMEM:
		exceptionClass = "android/database/sqlite/SQLiteOutOfMemoryException";
		break;
	case SQLITE_MISMATCH:
		exceptionClass = "android/database/sqlite/SQLiteDatatypeMismatchException";
		break;
	case SQLITE_INTERRUPT:
		exceptionClass = "android/os/OperationCanceledException";
		break;
	default:
		exceptionClass = "android/database/sqlite/SQLiteException";
		break;
	}

	if (sqlite3Message) {
		char *zFullmsg = sqlite3_mprintf(
			"%s (code %d)%s%s", sqlite3Message, errcode, 
			(message ? ": " : ""), (message ? message : "")
		);
		printf("throwing new %s\n", exceptionClass);
		(*env)->ThrowNew(env, (*env)->FindClass(env, exceptionClass), zFullmsg);
		sqlite3_free(zFullmsg);
	} else {
		printf("throwing new %s\n", exceptionClass);
		(*env)->ThrowNew(env, (*env)->FindClass(env, exceptionClass), message);
	}
}
