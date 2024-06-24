package android.os;

public class Binder extends IBinder {

	public void attachInterface(IInterface owner, String descriptor) {}

	public static void flushPendingCommands() {}

	public static long clearCallingIdentity() { return 0; }

	public static void restoreCallingIdentity(long identityToken) {}
}
