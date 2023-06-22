package android.view;

public final class Choreographer {
	public static interface FrameCallback {
		public void doFrame(long frametime_in_nanoseconds);
	}

	public static Choreographer getInstance() {
		return new Choreographer();
	}

	public void postFrameCallback(Choreographer.FrameCallback callback) {
		postFrameCallbackDelayed(callback, 0);
	}

	public void postFrameCallbackDelayed(final Choreographer.FrameCallback callback, long delayMillis) {
		// TODO - do the delay part
		// NOTE: if we do this synchronously, it gets stuck
		Thread async = new Thread(new Runnable() {
		public void run() {
			callback.doFrame(System.nanoTime());
		} });
		async.start();
	}
}
