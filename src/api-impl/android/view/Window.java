package android.view;

import android.content.Context;
import android.graphics.drawable.Drawable;
import android.view.SurfaceHolder;

public class Window {
	public static final int FEATURE_OPTIONS_PANEL = 0;

	public static interface Callback {
		public void onContentChanged();

		public abstract boolean onCreatePanelMenu(int featureId, Menu menu);

		public View onCreatePanelView(int featureId);

		public boolean onPreparePanel(int featureId, View view, Menu menu);

		public boolean onMenuItemSelected(int featureId, MenuItem item);

		public void onPanelClosed(int featureId, Menu menu);

		public boolean onMenuOpened(int featureId, Menu menu);
	}

	public long native_window;
	public View contentView;

	private Window.Callback callback;

	public Window(Window.Callback callback) {
		this.callback = callback;
		contentView = new ViewGroup(Context.this_application);
		contentView.setId(android.R.id.content);
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
		view.onAttachedToWindow();
		set_widget_as_root(native_window, view.widget);
	}

	public View getDecorView() {
		if (contentView != null)
			return contentView;
		return new View(Context.this_application); // FIXME: this can probably backfire
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
		if (contentView != null)
			return contentView.findViewById(id);
		else
			return null;
	}

	public View peekDecorView() {
		return null;
	}

	public WindowManager.LayoutParams getAttributes() {
		return new WindowManager.LayoutParams();
	}

	public void setBackgroundDrawable(Drawable drawable) {
		/* TODO: should this be *under* the contentView background? */
		if (contentView != null)
			contentView.setBackgroundDrawable(drawable);
	}

	public void setAttributes(WindowManager.LayoutParams params) {}

	public void takeSurface(SurfaceHolder.Callback2 callback) {}

	public void setStatusBarColor(int color) {}

	public void setNavigationBarColor(int color) {}

	public void setFormat(int format) {}

	public void setLayout(int dummy, int dummy2) {}

	public WindowManager getWindowManager() {
		return new WindowManagerImpl();
	}

	public void setSoftInputMode(int dummy) {}

	public int getNavigationBarColor() {
		return 0xFF888888;  // gray
	}

	public void setBackgroundDrawableResource(int resId) {}

	public int getStatusBarColor() { return 0xFFFF0000; }
}
