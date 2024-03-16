package android.app;


import android.content.Context;
import android.content.ComponentName;

public class ApplicationErrorReport {
	public static ComponentName getErrorReportReceiver(Context context, String packageName, int appFlags)
	{
		return new ComponentName(context, packageName);
	}

}
