/*
 * Copyright (C) 2006 The Android Open Source Project
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

package android.os;

import android.util.Log;
import java.io.BufferedWriter;
import java.io.DataInputStream;
import java.io.IOException;
import java.io.OutputStreamWriter;
import java.util.ArrayList;
import libcore.io.Libcore;

/**
 * Tools for managing OS processes.
 */
public class Process {
	private static final String LOG_TAG = "Process";

	/**
	 * Name of a process for running the platform's media services.
	 * {@hide}
	 */
	public static final String ANDROID_SHARED_MEDIA = "com.android.process.media";

	/**
	 * Name of the process that Google content providers can share.
	 * {@hide}
	 */
	public static final String GOOGLE_SHARED_APP_CONTENT = "com.google.process.content";

	/**
	 * Defines the UID/GID under which system code runs.
	 */
	public static final int SYSTEM_UID = 1000;

	/**
	 * Defines the UID/GID under which the telephony code runs.
	 */
	public static final int PHONE_UID = 1001;

	/**
	 * Defines the UID/GID for the user shell.
	 * @hide
	 */
	public static final int SHELL_UID = 2000;

	/**
	 * Defines the UID/GID for the log group.
	 * @hide
	 */
	public static final int LOG_UID = 1007;

	/**
	 * Defines the UID/GID for the WIFI supplicant process.
	 * @hide
	 */
	public static final int WIFI_UID = 1010;

	/**
	 * Defines the UID/GID for the mediaserver process.
	 * @hide
	 */
	public static final int MEDIA_UID = 1013;

	/**
	 * Defines the UID/GID for the DRM process.
	 * @hide
	 */
	public static final int DRM_UID = 1019;

	/**
	 * Defines the UID/GID for the group that controls VPN services.
	 * @hide
	 */
	public static final int VPN_UID = 1016;

	/**
	 * Defines the UID/GID for the NFC service process.
	 * @hide
	 */
	public static final int NFC_UID = 1027;

	/**
	 * Defines the UID/GID for the Bluetooth service process.
	 * @hide
	 */
	public static final int BLUETOOTH_UID = 1002;

	/**
	 * Defines the GID for the group that allows write access to the internal media storage.
	 * @hide
	 */
	public static final int MEDIA_RW_GID = 1023;

	/**
	 * Access to installed package details
	 * @hide
	 */
	public static final int PACKAGE_INFO_GID = 1032;

	/**
	 * Defines the start of a range of UIDs (and GIDs), going from this
	 * number to {@link #LAST_APPLICATION_UID} that are reserved for assigning
	 * to applications.
	 */
	public static final int FIRST_APPLICATION_UID = 10000;

	/**
	 * Last of application-specific UIDs starting at
	 * {@link #FIRST_APPLICATION_UID}.
	 */
	public static final int LAST_APPLICATION_UID = 19999;

	/**
	 * First uid used for fully isolated sandboxed processes (with no permissions of their own)
	 * @hide
	 */
	public static final int FIRST_ISOLATED_UID = 99000;

	/**
	 * Last uid used for fully isolated sandboxed processes (with no permissions of their own)
	 * @hide
	 */
	public static final int LAST_ISOLATED_UID = 99999;

	/**
	 * First gid for applications to share resources. Used when forward-locking
	 * is enabled but all UserHandles need to be able to read the resources.
	 * @hide
	 */
	public static final int FIRST_SHARED_APPLICATION_GID = 50000;

	/**
	 * Last gid for applications to share resources. Used when forward-locking
	 * is enabled but all UserHandles need to be able to read the resources.
	 * @hide
	 */
	public static final int LAST_SHARED_APPLICATION_GID = 59999;

	/**
	 * Standard priority of application threads.
	 * Use with {@link #setThreadPriority(int)} and
	 * {@link #setThreadPriority(int, int)}, <b>not</b> with the normal
	 * {@link java.lang.Thread} class.
	 */
	public static final int THREAD_PRIORITY_DEFAULT = 0;

	/*
	 * ***************************************
	 * ** Keep in sync with utils/threads.h **
	 * ***************************************
	 */

	/**
	 * Lowest available thread priority.  Only for those who really, really
	 * don't want to run if anything else is happening.
	 * Use with {@link #setThreadPriority(int)} and
	 * {@link #setThreadPriority(int, int)}, <b>not</b> with the normal
	 * {@link java.lang.Thread} class.
	 */
	public static final int THREAD_PRIORITY_LOWEST = 19;

	/**
	 * Standard priority background threads.  This gives your thread a slightly
	 * lower than normal priority, so that it will have less chance of impacting
	 * the responsiveness of the user interface.
	 * Use with {@link #setThreadPriority(int)} and
	 * {@link #setThreadPriority(int, int)}, <b>not</b> with the normal
	 * {@link java.lang.Thread} class.
	 */
	public static final int THREAD_PRIORITY_BACKGROUND = 10;

	/**
	 * Standard priority of threads that are currently running a user interface
	 * that the user is interacting with.  Applications can not normally
	 * change to this priority; the system will automatically adjust your
	 * application threads as the user moves through the UI.
	 * Use with {@link #setThreadPriority(int)} and
	 * {@link #setThreadPriority(int, int)}, <b>not</b> with the normal
	 * {@link java.lang.Thread} class.
	 */
	public static final int THREAD_PRIORITY_FOREGROUND = -2;

	/**
	 * Standard priority of system display threads, involved in updating
	 * the user interface.  Applications can not
	 * normally change to this priority.
	 * Use with {@link #setThreadPriority(int)} and
	 * {@link #setThreadPriority(int, int)}, <b>not</b> with the normal
	 * {@link java.lang.Thread} class.
	 */
	public static final int THREAD_PRIORITY_DISPLAY = -4;

	/**
	 * Standard priority of the most important display threads, for compositing
	 * the screen and retrieving input events.  Applications can not normally
	 * change to this priority.
	 * Use with {@link #setThreadPriority(int)} and
	 * {@link #setThreadPriority(int, int)}, <b>not</b> with the normal
	 * {@link java.lang.Thread} class.
	 */
	public static final int THREAD_PRIORITY_URGENT_DISPLAY = -8;

	/**
	 * Standard priority of audio threads.  Applications can not normally
	 * change to this priority.
	 * Use with {@link #setThreadPriority(int)} and
	 * {@link #setThreadPriority(int, int)}, <b>not</b> with the normal
	 * {@link java.lang.Thread} class.
	 */
	public static final int THREAD_PRIORITY_AUDIO = -16;

	/**
	 * Standard priority of the most important audio threads.
	 * Applications can not normally change to this priority.
	 * Use with {@link #setThreadPriority(int)} and
	 * {@link #setThreadPriority(int, int)}, <b>not</b> with the normal
	 * {@link java.lang.Thread} class.
	 */
	public static final int THREAD_PRIORITY_URGENT_AUDIO = -19;

	/**
	 * Minimum increment to make a priority more favorable.
	 */
	public static final int THREAD_PRIORITY_MORE_FAVORABLE = -1;

	/**
	 * Minimum increment to make a priority less favorable.
	 */
	public static final int THREAD_PRIORITY_LESS_FAVORABLE = +1;

	/**
	 * Default scheduling policy
	 * @hide
	 */
	public static final int SCHED_OTHER = 0;

	/**
	 * First-In First-Out scheduling policy
	 * @hide
	 */
	public static final int SCHED_FIFO = 1;

	/**
	 * Round-Robin scheduling policy
	 * @hide
	 */
	public static final int SCHED_RR = 2;

	/**
	 * Batch scheduling policy
	 * @hide
	 */
	public static final int SCHED_BATCH = 3;

	/**
	 * Idle scheduling policy
	 * @hide
	 */
	public static final int SCHED_IDLE = 5;

	// Keep in sync with SP_* constants of enum type SchedPolicy
	// declared in system/core/include/cutils/sched_policy.h,
	// except THREAD_GROUP_DEFAULT does not correspond to any SP_* value.

	/**
	 * Default thread group -
	 * has meaning with setProcessGroup() only, cannot be used with setThreadGroup().
	 * When used with setProcessGroup(), the group of each thread in the process
	 * is conditionally changed based on that thread's current priority, as follows:
	 * threads with priority numerically less than THREAD_PRIORITY_BACKGROUND
	 * are moved to foreground thread group.  All other threads are left unchanged.
	 * @hide
	 */
	public static final int THREAD_GROUP_DEFAULT = -1;

	/**
	 * Background thread group - All threads in
	 * this group are scheduled with a reduced share of the CPU.
	 * Value is same as constant SP_BACKGROUND of enum SchedPolicy.
	 * FIXME rename to THREAD_GROUP_BACKGROUND.
	 * @hide
	 */
	public static final int THREAD_GROUP_BG_NONINTERACTIVE = 0;

	/**
	 * Foreground thread group - All threads in
	 * this group are scheduled with a normal share of the CPU.
	 * Value is same as constant SP_FOREGROUND of enum SchedPolicy.
	 * Not used at this level.
	 * @hide
	 **/
	private static final int THREAD_GROUP_FOREGROUND = 1;

	/**
	 * System thread group.
	 * @hide
	 **/
	public static final int THREAD_GROUP_SYSTEM = 2;

	/**
	 * Application audio thread group.
	 * @hide
	 **/
	public static final int THREAD_GROUP_AUDIO_APP = 3;

	/**
	 * System audio thread group.
	 * @hide
	 **/
	public static final int THREAD_GROUP_AUDIO_SYS = 4;

	public static final int SIGNAL_QUIT = 3;
	public static final int SIGNAL_KILL = 9;
	public static final int SIGNAL_USR1 = 10;

	/**
	 * Start a new process.
	 *
	 * <p>If processes are enabled, a new process is created and the
	 * static main() function of a <var>processClass</var> is executed there.
	 * The process will continue running after this function returns.
	 *
	 * <p>If processes are not enabled, a new thread in the caller's
	 * process is created and main() of <var>processClass</var> called there.
	 *
	 * <p>The niceName parameter, if not an empty string, is a custom name to
	 * give to the process instead of using processClass.  This allows you to
	 * make easily identifyable processes even if you are using the same base
	 * <var>processClass</var> to start them.
	 *
	 * @param processClass The class to use as the process's main entry
	 *                     point.
	 * @param niceName A more readable name to use for the process.
	 * @param uid The user-id under which the process will run.
	 * @param gid The group-id under which the process will run.
	 * @param gids Additional group-ids associated with the process.
	 * @param debugFlags Additional flags.
	 * @param targetSdkVersion The target SDK version for the app.
	 * @param seInfo null-ok SE Android information for the new process.
	 * @param zygoteArgs Additional arguments to supply to the zygote process.
	 *
	 * @return An object that describes the result of the attempt to start the process.
	 * @throws RuntimeException on fatal start failure
	 *
	 * {@hide}
	 */
	public static final ProcessStartResult start(final String processClass,
						     final String niceName,
						     int uid, int gid, int[] gids,
						     int debugFlags, int mountExternal,
						     int targetSdkVersion,
						     String seInfo,
						     String[] zygoteArgs) {
		System.out.println("NOT starting a process");

		return null;
	}

	/**
	 * retry interval for opening a zygote socket
	 */
	static final int ZYGOTE_RETRY_MILLIS = 500;

	/**
	 * Returns elapsed milliseconds of the time this process has run.
	 * @return  Returns the number of milliseconds this process has return.
	 */
	public static final native long getElapsedCpuTime();

	/**
	 * Returns the identifier of this process, which can be used with
	 * {@link #killProcess} and {@link #sendSignal}.
	 */
	public static final int myPid() {
		return Libcore.os.getpid();
	}

	/**
	 * Returns the identifier of this process' parent.
	 * @hide
	 */
	public static final int myPpid() {
		return Libcore.os.getppid();
	}

	/**
	 * Returns the identifier of the calling thread, which be used with
	 * {@link #setThreadPriority(int, int)}.
	 */
	public static final int myTid() {
		return Libcore.os.gettid();
	}

	/**
	 * Returns the identifier of this process's uid.  This is the kernel uid
	 * that the process is running under, which is the identity of its
	 * app-specific sandbox.  It is different from {@link #myUserHandle} in that
	 * a uid identifies a specific app sandbox in a specific user.
	 */
	public static final int myUid() {
		// HACK: provide wrong Uid, as some applications like Whatsapp don't accept files with their own Uid for security reasons
		return -1;
		// return Libcore.os.getuid();
	}

	/**
	 * Returns this process's user handle.  This is the
	 * user the process is running under.  It is distinct from
	 * {@link #myUid()} in that a particular user will have multiple
	 * distinct apps running under it each with their own uid.
	 */
	public static final UserHandle myUserHandle() {
		return new UserHandle(UserHandle.getUserId(myUid()));
	}

	/**
	 * Returns whether the current process is in an isolated sandbox.
	 * @hide
	 */
	public static final boolean isIsolated() {
		return false;
	}

	/**
	 * Returns the UID assigned to a particular user name, or -1 if there is
	 * none.  If the given string consists of only numbers, it is converted
	 * directly to a uid.
	 */
	public static final native int getUidForName(String name);

	/**
	 * Returns the GID assigned to a particular user name, or -1 if there is
	 * none.  If the given string consists of only numbers, it is converted
	 * directly to a gid.
	 */
	public static final native int getGidForName(String name);

	/**
	 * Returns a uid for a currently running process.
	 * @param pid the process id
	 * @return the uid of the process, or -1 if the process is not running.
	 * @hide pending API council review
	 */
	public static final int getUidForPid(int pid) {
		String[] procStatusLabels = {"Uid:"};
		long[] procStatusValues = new long[1];
		procStatusValues[0] = -1;
		Process.readProcLines("/proc/" + pid + "/status", procStatusLabels, procStatusValues);
		return (int)procStatusValues[0];
	}

	/**
	 * Returns the parent process id for a currently running process.
	 * @param pid the process id
	 * @return the parent process id of the process, or -1 if the process is not running.
	 * @hide
	 */
	public static final int getParentPid(int pid) {
		String[] procStatusLabels = {"PPid:"};
		long[] procStatusValues = new long[1];
		procStatusValues[0] = -1;
		Process.readProcLines("/proc/" + pid + "/status", procStatusLabels, procStatusValues);
		return (int)procStatusValues[0];
	}

	/**
	 * Returns the thread group leader id for a currently running thread.
	 * @param tid the thread id
	 * @return the thread group leader id of the thread, or -1 if the thread is not running.
	 *         This is same as what getpid(2) would return if called by tid.
	 * @hide
	 */
	public static final int getThreadGroupLeader(int tid) {
		String[] procStatusLabels = {"Tgid:"};
		long[] procStatusValues = new long[1];
		procStatusValues[0] = -1;
		Process.readProcLines("/proc/" + tid + "/status", procStatusLabels, procStatusValues);
		return (int)procStatusValues[0];
	}

	/**
	 * Set the priority of a thread, based on Linux priorities.
	 *
	 * @param tid The identifier of the thread/process to change.
	 * @param priority A Linux priority level, from -20 for highest scheduling
	 * priority to 19 for lowest scheduling priority.
	 *
	 * @throws IllegalArgumentException Throws IllegalArgumentException if
	 * <var>tid</var> does not exist.
	 * @throws SecurityException Throws SecurityException if your process does
	 * not have permission to modify the given thread, or to use the given
	 * priority.
	 */
	public static final /*native*/ void setThreadPriority(int tid, int priority)
	    throws IllegalArgumentException, SecurityException {
		// TODO - this is clearly not critical
	}

	/**
	 * Call with 'false' to cause future calls to {@link #setThreadPriority(int)} to
	 * throw an exception if passed a background-level thread priority.  This is only
	 * effective if the JNI layer is built with GUARD_THREAD_PRIORITY defined to 1.
	 *
	 * @hide
	 */
	public static final native void setCanSelfBackground(boolean backgroundOk);

	/**
	 * Sets the scheduling group for a thread.
	 * @hide
	 * @param tid The identifier of the thread to change.
	 * @param group The target group for this thread from THREAD_GROUP_*.
	 *
	 * @throws IllegalArgumentException Throws IllegalArgumentException if
	 * <var>tid</var> does not exist.
	 * @throws SecurityException Throws SecurityException if your process does
	 * not have permission to modify the given thread, or to use the given
	 * priority.
	 * If the thread is a thread group leader, that is it's gettid() == getpid(),
	 * then the other threads in the same thread group are _not_ affected.
	 */
	public static final native void setThreadGroup(int tid, int group)
	    throws IllegalArgumentException, SecurityException;

	/**
	 * Sets the scheduling group for a process and all child threads
	 * @hide
	 * @param pid The identifier of the process to change.
	 * @param group The target group for this process from THREAD_GROUP_*.
	 *
	 * @throws IllegalArgumentException Throws IllegalArgumentException if
	 * <var>tid</var> does not exist.
	 * @throws SecurityException Throws SecurityException if your process does
	 * not have permission to modify the given thread, or to use the given
	 * priority.
	 *
	 * group == THREAD_GROUP_DEFAULT means to move all non-background priority
	 * threads to the foreground scheduling group, but to leave background
	 * priority threads alone.  group == THREAD_GROUP_BG_NONINTERACTIVE moves all
	 * threads, regardless of priority, to the background scheduling group.
	 * group == THREAD_GROUP_FOREGROUND is not allowed.
	 */
	public static final native void setProcessGroup(int pid, int group)
	    throws IllegalArgumentException, SecurityException;

	/**
	 * Return the scheduling group of requested process.
	 *
	 * @hide
	 */
	public static final native int getProcessGroup(int pid)
	    throws IllegalArgumentException, SecurityException;

	/**
	 * Set the priority of the calling thread, based on Linux priorities.  See
	 * {@link #setThreadPriority(int, int)} for more information.
	 *
	 * @param priority A Linux priority level, from -20 for highest scheduling
	 * priority to 19 for lowest scheduling priority.
	 *
	 * @throws IllegalArgumentException Throws IllegalArgumentException if
	 * <var>tid</var> does not exist.
	 * @throws SecurityException Throws SecurityException if your process does
	 * not have permission to modify the given thread, or to use the given
	 * priority.
	 *
	 * @see #setThreadPriority(int, int)
	 */
	public static final /*native*/ void setThreadPriority(int priority)
	    throws IllegalArgumentException, SecurityException {}

	/**
	 * Return the current priority of a thread, based on Linux priorities.
	 *
	 * @param tid The identifier of the thread/process to change.
	 *
	 * @return Returns the current priority, as a Linux priority level,
	 * from -20 for highest scheduling priority to 19 for lowest scheduling
	 * priority.
	 *
	 * @throws IllegalArgumentException Throws IllegalArgumentException if
	 * <var>tid</var> does not exist.
	 */
	public static final native int getThreadPriority(int tid)
	    throws IllegalArgumentException;

	/**
	 * Set the scheduling policy and priority of a thread, based on Linux.
	 *
	 * @param tid The identifier of the thread/process to change.
	 * @param policy A Linux scheduling policy such as SCHED_OTHER etc.
	 * @param priority A Linux priority level in a range appropriate for the given policy.
	 *
	 * @throws IllegalArgumentException Throws IllegalArgumentException if
	 * <var>tid</var> does not exist, or if <var>priority</var> is out of range for the policy.
	 * @throws SecurityException Throws SecurityException if your process does
	 * not have permission to modify the given thread, or to use the given
	 * scheduling policy or priority.
	 *
	 * {@hide}
	 */
	public static final native void setThreadScheduler(int tid, int policy, int priority)
	    throws IllegalArgumentException;

	/**
	 * Determine whether the current environment supports multiple processes.
	 *
	 * @return Returns true if the system can run in multiple processes, else
	 * false if everything is running in a single process.
	 *
	 * @deprecated This method always returns true.  Do not use.
	 */
	@Deprecated
	public static final boolean supportsProcesses() {
		return true;
	}

	/**
	 * Set the out-of-memory badness adjustment for a process.
	 *
	 * @param pid The process identifier to set.
	 * @param amt Adjustment value -- linux allows -16 to +15.
	 *
	 * @return Returns true if the underlying system supports this
	 *         feature, else false.
	 *
	 * {@hide}
	 */
	public static final native boolean setOomAdj(int pid, int amt);

	/**
	 * Adjust the swappiness level for a process.
	 *
	 * @param pid The process identifier to set.
	 * @param is_increased Whether swappiness should be increased or default.
	 *
	 * @return Returns true if the underlying system supports this
	 *         feature, else false.
	 *
	 * {@hide}
	 */
	public static final native boolean setSwappiness(int pid, boolean is_increased);

	/**
	 * Change this process's argv[0] parameter.  This can be useful to show
	 * more descriptive information in things like the 'ps' command.
	 *
	 * @param text The new name of this process.
	 *
	 * {@hide}
	 */
	public static final native void setArgV0(String text);

	/**
	 * Kill the process with the given PID.
	 * Note that, though this API allows us to request to
	 * kill any process based on its PID, the kernel will
	 * still impose standard restrictions on which PIDs you
	 * are actually able to kill.  Typically this means only
	 * the process running the caller's packages/application
	 * and any additional processes created by that app; packages
	 * sharing a common UID will also be able to kill each
	 * other's processes.
	 */
	public static final void killProcess(int pid) {
		if(pid == Process.myPid()) {
			System.out.println("the app called killProcess on itself");
			System.exit(0);
		}

		sendSignal(pid, SIGNAL_KILL);
	}

	/**
	 * @hide
	 */
	public static final native int setUid(int uid);

	/**
	 * @hide
	 */
	public static final native int setGid(int uid);

	/**
	 * Send a signal to the given process.
	 *
	 * @param pid The pid of the target process.
	 * @param signal The signal to send.
	 */
	public static final native void sendSignal(int pid, int signal);

	/**
	 * @hide
	 * Private impl for avoiding a log message...  DO NOT USE without doing
	 * your own log, or the Android Illuminati will find you some night and
	 * beat you up.
	 */
	public static final void killProcessQuiet(int pid) {
		sendSignalQuiet(pid, SIGNAL_KILL);
	}

	/**
	 * @hide
	 * Private impl for avoiding a log message...  DO NOT USE without doing
	 * your own log, or the Android Illuminati will find you some night and
	 * beat you up.
	 */
	public static final native void sendSignalQuiet(int pid, int signal);

	/**
	 * @hide
	 */
	public static final native long getFreeMemory();

	/**
	 * @hide
	 */
	public static final native long getTotalMemory();

	/**
	 * @hide
	 */
	public static final native void readProcLines(String path,
						      String[] reqFields, long[] outSizes);

	/**
	 * @hide
	 */
	public static final native int[] getPids(String path, int[] lastArray);

	/**
	 * @hide
	 */
	public static final int PROC_TERM_MASK = 0xff;
	/**
	 * @hide
	 */
	public static final int PROC_ZERO_TERM = 0;
	/**
	 * @hide
	 */
	public static final int PROC_SPACE_TERM = (int)' ';
	/**
	 * @hide
	 */
	public static final int PROC_TAB_TERM = (int)'\t';
	/**
	 * @hide
	 */
	public static final int PROC_COMBINE = 0x100;
	/**
	 * @hide
	 */
	public static final int PROC_PARENS = 0x200;
	/**
	 * @hide
	 */
	public static final int PROC_QUOTES = 0x400;
	/**
	 * @hide
	 */
	public static final int PROC_OUT_STRING = 0x1000;
	/**
	 * @hide
	 */
	public static final int PROC_OUT_LONG = 0x2000;
	/**
	 * @hide
	 */
	public static final int PROC_OUT_FLOAT = 0x4000;

	/**
	 * @hide
	 */
	public static final /*native*/ boolean readProcFile(String file, int[] format,
							String[] outStrings, long[] outLongs, float[] outFloats) {
		System.out.println("readProcFile(" + file + ") called");
		return false;
	}

	/**
	 * @hide
	 */
	public static final native boolean parseProcLine(byte[] buffer, int startIndex,
							 int endIndex, int[] format, String[] outStrings, long[] outLongs, float[] outFloats);

	/**
	 * @hide
	 */
	public static final native int[] getPidsForCommands(String[] cmds);

	/**
	 * Gets the total Pss value for a given process, in bytes.
	 *
	 * @param pid the process to the Pss for
	 * @return the total Pss value for the given process in bytes,
	 *  or -1 if the value cannot be determined
	 * @hide
	 */
	public static final native long getPss(int pid);

	/**
	 * Specifies the outcome of having started a process.
	 * @hide
	 */
	public static final class ProcessStartResult {
		/**
		 * The PID of the newly started process.
		 * Always >= 0.  (If the start failed, an exception will have been thrown instead.)
		 */
		public int pid;

		/**
		 * True if the process was started with a wrapper attached.
		 */
		public boolean usingWrapper;
	}

    public static final boolean is64Bit() {
        return true;
    }
}
