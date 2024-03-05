package android.app;

import android.content.Context;
import android.content.DialogInterface;
import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.Window;

public class Dialog implements Window.Callback, DialogInterface {
	protected long nativePtr;

	private native long nativeInit();
	private native void nativeSetTitle(long ptr, String title);
	private native void nativeSetContentView(long ptr, long widget);
	private native void nativeShow(long ptr);
	private native void nativeClose(long ptr);

	private Context context;
	private Window window;
	private OnDismissListener onDismissListener;

	public Dialog(Context context, int themeResId) {
		this.context = context;
		nativePtr = nativeInit();
		window = new Window(this);
		window.native_window = nativePtr;
	}

	public final boolean requestWindowFeature(int featureId) {
		return false;
	}

	public Context getContext() {
		return context;
	}

	public void setContentView(View view) {
		getWindow().setContentView(view);
	}

	public void setTitle(CharSequence title) {
		nativeSetTitle(nativePtr, String.valueOf(title));
	}

	public void setTitle(int titleId) {
		nativeSetTitle(nativePtr, context.getString(titleId));
	}

	public void setOwnerActivity(Activity activity) {}

	public void setCancelable(boolean cancelable) {}

	public void setOnCancelListener(OnCancelListener onCancelListener) {}

	public void setOnDismissListener(OnDismissListener onDismissListener) {
		this.onDismissListener = onDismissListener;
	}

	public void show() {
		System.out.println("showing the Dialog " + this);
		new Handler(Looper.getMainLooper()).post(new Runnable() {
			@Override
			public void run() {
				onCreate(null);
				nativeShow(nativePtr);
			}
		});
	}

	public boolean isShowing() {
		return false;
	}

	public void dismiss() {
		System.out.println("dismissing the Dialog " + Dialog.this);
		new Handler(Looper.getMainLooper()).post(new Runnable() {
			@Override
			public void run() {
				nativeClose(nativePtr);
				if (onDismissListener != null)
					onDismissListener.onDismiss(Dialog.this);
			}
		});
	}

	public Window getWindow() {
		return window;
	}

	public void setCanceledOnTouchOutside(boolean cancel) {}

	public class Builder {
		public Builder(Context context) {
			System.out.println("making a Dialog$Builder");
		}
	}

	@Override
	public void onContentChanged() {
	}
	@Override
	public boolean onCreatePanelMenu(int featureId, Menu menu) {
		// TODO Auto-generated method stub
		throw new UnsupportedOperationException("Unimplemented method 'onCreatePanelMenu'");
	}
	@Override
	public View onCreatePanelView(int featureId) {
		// TODO Auto-generated method stub
		throw new UnsupportedOperationException("Unimplemented method 'onCreatePanelView'");
	}
	@Override
	public boolean onPreparePanel(int featureId, View view, Menu menu) {
		// TODO Auto-generated method stub
		throw new UnsupportedOperationException("Unimplemented method 'onPreparePanel'");
	}
	@Override
	public boolean onMenuItemSelected(int featureId, MenuItem item) {
		// TODO Auto-generated method stub
		throw new UnsupportedOperationException("Unimplemented method 'onMenuItemSelected'");
	}
	@Override
	public void onPanelClosed(int featureId, Menu menu) {
		// TODO Auto-generated method stub
		throw new UnsupportedOperationException("Unimplemented method 'onPanelClosed'");
	}

	@Override
	public boolean onMenuOpened(int featureId, Menu menu) {
		// TODO Auto-generated method stub
		throw new UnsupportedOperationException("Unimplemented method 'onMenuOpened'");
	}

	protected void onCreate (Bundle savedInstanceState) {
		System.out.println("- onCreate - Dialog!");
	}

	public void hide() {
		System.out.println("hiding the Dialog " + this);
		nativeClose(nativePtr);
	}
}
