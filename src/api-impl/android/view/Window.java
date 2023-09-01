package android.view;

public class Window {
	public static final int FEATURE_OPTIONS_PANEL = 0;

	public static interface Callback {
		public void onContentChanged();

		public abstract boolean onCreatePanelMenu(int featureId, Menu menu);

		public View onCreatePanelView(int featureId);

		public boolean onPreparePanel(int featureId, View view, Menu menu);

		public boolean onMenuItemSelected(int featureId, MenuItem item);

		public void onPanelClosed(int featureId, Menu menu);
	}

	// FIXME private
	public long native_window;
	public View contentView;

	private Window.Callback callback;

	public Window(Window.Callback callback) {
		this.callback = callback;
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
		contentView = view;
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

	public WindowManager.LayoutParams getAttributes() {
		return new WindowManager.LayoutParams();
	}

	public void setAttributes(WindowManager.LayoutParams params) {}
}
