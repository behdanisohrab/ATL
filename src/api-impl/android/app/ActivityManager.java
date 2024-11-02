package android.app;

import android.content.pm.ConfigurationInfo;
import android.graphics.Bitmap;
import android.os.IBinder;
import android.os.Parcel;
import android.os.Parcelable;

import java.util.ArrayList;
import java.util.List;

public class ActivityManager {

	public static class RunningAppProcessInfo{
		public int importance;
	}

	public static class TaskDescription {
		public TaskDescription(String name, Bitmap icon, int color) {}
	}

	public List<RunningAppProcessInfo> getRunningAppProcesses() {
		return null;
	}

	public boolean isLowRamDevice() {return false;}

	public static class MemoryInfo {
		/* For now, just always report there's 10GB free RAM */
		public long availMem = 10000;

		public long totalMem = 10000;

		public long threshold = 200;

		public boolean lowMemory = false;
	}

	public void getMemoryInfo(MemoryInfo outInfo)
	{
		outInfo = new MemoryInfo();
	}

	public ConfigurationInfo getDeviceConfigurationInfo() {
		return new ConfigurationInfo();
	}

	public int getMemoryClass() {return 20;}  // suggested heap size in MB

	public static void getMyMemoryState(RunningAppProcessInfo outInfo) {}

	public boolean clearApplicationUserData() {return false;}

	public static class AppTask {}
	public List<ActivityManager.AppTask> getAppTasks() {
		return new ArrayList<>();
	}
	
	public static class RunningServiceInfo implements Parcelable {
		public RunningServiceInfo() {
		}

		public int describeContents() {
			return 0;
		}

		public void writeToParcel(Parcel dest, int flags) {
			return;
		}

		public void readFromParcel(Parcel source) {
			return;
		}
	}     


	public List<RunningServiceInfo> getRunningServices(int maxNum)
		throws SecurityException {
			return new ArrayList<>();
	}
}
