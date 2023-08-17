package android.view;

public class Window {
	public static interface Callback {
		public void onContentChanged();

		public abstract boolean onCreatePanelMenu(int featureId, Menu menu);
	}
	public static class fixme_callback implements Callback {

		@Override
		public void onContentChanged() {
		}

		@Override
		public boolean onCreatePanelMenu(int featureId, Menu menu) {
			return false;
		}
	}

	// FIXME private
	public long native_window;

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
		set_widget_as_root(native_window, view.widget);
	}

	public View getDecorView() {
		return new View(); // FIXME: this can probably backfire
	}

	private native void set_widget_as_root(long native_window, long widget);

	public native void take_input_queue(long native_window, InputQueue.Callback callback, InputQueue queue);

	public void takeInputQueue(InputQueue.Callback callback) {
		take_input_queue(native_window, callback, new InputQueue());
	}

	public boolean requestFeature(int featureId) {
		return false;
	}

	public View findViewById(int id) {
		return View.view_by_id.get(id);
	}

	public View peekDecorView() {
		return null;
	}
}
