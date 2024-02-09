package android.app;

import android.content.Context;
import android.content.DialogInterface.OnCancelListener;
import android.content.DialogInterface.OnDismissListener;
import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.Window;

public class Dialog implements Window.Callback {
	protected long nativePtr;

	private native long nativeInit();
	private native void nativeSetTitle(long ptr, String title);
	private native void nativeSetContentView(long ptr, long widget);
	private native void nativeShow(long ptr);

	private Context context;

	public Dialog(Context context, int themeResId) {
		this.context = context;
		nativePtr = nativeInit();
	}

	public final boolean requestWindowFeature(int featureId) {
		return false;
	}

	public Context getContext() {
		return context;
	}

	public void setContentView(View view) {
		nativeSetContentView(nativePtr, view.widget);
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

	public void setOnDismissListener(OnDismissListener onDismissListener) {}

	public void show() {
		System.out.println("totally showing the Dialog " + this + " right now, most definitely doing that");
		onCreate(null);
		new Handler(Looper.getMainLooper()).post(new Runnable() {
			@Override
			public void run() {
				nativeShow(nativePtr);
			}
		});
	}

	public boolean isShowing() {
		return false;
	}

	public void dismiss() {
		System.out.println("totally dismissing the Dialog " + this + " right now, which was most definitely being shown just a moment ago");
	}

	public Window getWindow() {
		return new Window(this) {
			@Override
			public void setContentView(View view) {
				Dialog.this.setContentView(view);
			}
		};
	}

	public void setCanceledOnTouchOutside(boolean cancel) {}

	public class Builder {
		public Builder(Context context) {
			System.out.println("making a Dialog$Builder as we speak, my word!");
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

	protected void onCreate (Bundle savedInstanceState) {
		System.out.println("- onCreate - Dialog!");
	}
}
