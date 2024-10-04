package android.app;

import android.content.pm.ConfigurationInfo;
import android.graphics.Bitmap;

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
}
