package android.app;

import java.util.List;

public class ActivityManager {

	public static class RunningAppProcessInfo{}

	public List<RunningAppProcessInfo> getRunningAppProcesses() {
		return null;
	}

	public boolean isLowRamDevice() {return false;}

	public static class MemoryInfo {
		/* For now, just always report there's 10GB free RAM */
		public long availMem = 10000;
	}

	public void getMemoryInfo (MemoryInfo outInfo)
	{
		outInfo = new MemoryInfo();
	}
}
