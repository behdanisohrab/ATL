package android.app;

import android.app.Application;
import android.content.pm.ConfigurationInfo;
import android.content.Context;
import java.util.List;

public class ActivityThread {
	public static ActivityThread currentActivityThread() {
		return new ActivityThread();
	}
	public static String currentPackageName() {
		return Context.this_application.getPackageName();
	}
	public static Application currentApplication() {
		return Context.this_application;
	}
	public Application getApplication() {
		return Context.this_application;
	}
}
