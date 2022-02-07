package android.view;

public class Window {
	public static interface Callback {}
	public static class fixme_callback implements Callback {}

	private Window.Callback callback;

	public Window() {
		this.callback = new fixme_callback();
	}

	public void addFlags(int flags) {}
	public void setFlags(int flags, int mask) {}
	public void clearFlags(int flags) {}

	public final Callback getCallback() {
		return this.callback;
	}
	public void setCallback(Window.Callback callback) {
		this.callback = callback;
	}

	public void setContentView(View view) {
		set_widget_as_root(view.widget);
	}

	private native void set_widget_as_root(long widget);
}
