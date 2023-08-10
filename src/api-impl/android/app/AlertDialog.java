package android.app;

import android.content.Context;
import android.content.DialogInterface;
import android.os.Handler;
import android.os.Looper;
import android.view.View;

public class AlertDialog extends Dialog implements DialogInterface {

	private long nativePtr;

	private native long nativeInit();
	private native void nativeSetTitle(long ptr, String title);
	private native void nativeSetMessage(long ptr, String message);
	private native void nativeSetButton(long ptr, int whichButton, String text);
	private native void nativeSetItems(long ptr, String[] items, DialogInterface.OnClickListener listener);
	private native void nativeShow(long ptr);

	public AlertDialog(Context context) {
		nativePtr = nativeInit();
	}

	public void setTitle(CharSequence title) {
		nativeSetTitle(nativePtr, String.valueOf(title));
	}

	public void setOnCancelListener(OnCancelListener onCancelListener) {}

	public void setCancelable(boolean cancelable) {}

	public void setMessage(CharSequence message) {
		System.out.println("AlertDialog setMessage called with: '" + message + "'");
		nativeSetMessage(nativePtr, String.valueOf(message));
	}

	public void setButton(int whichButton, CharSequence text, OnClickListener listener) {
		nativeSetButton(nativePtr, whichButton, String.valueOf(text));
	}

	public boolean isShowing() {
		return false;
	}

	@Override
	public void show() {
		super.show();
		new Handler(Looper.getMainLooper()).post(new Runnable() {
			@Override
			public void run() {
				nativeShow(nativePtr);
			}
		});
	}

	public static class Builder {
		private Context context;
		private AlertDialog dialog;

		public Builder(Context context) {
			System.out.println("making an AlertDialog$Builder as we speak, my word!");
			this.context = context;
			dialog = new AlertDialog(context);
		}

		public AlertDialog.Builder setPositiveButton(int textId, DialogInterface.OnClickListener listener) {
			return setPositiveButton(context.getResources().getText(textId), listener);
		}

		public AlertDialog.Builder setPositiveButton(CharSequence text, DialogInterface.OnClickListener listener) {
			System.out.println("AlertDialog.Builder setPositiveButton called with text: '" + text + "'");
			dialog.setButton(DialogInterface.BUTTON_POSITIVE, text, listener);
			return this;
		}

		public AlertDialog.Builder setCancelable(boolean cancelable) {
			return this;
		}

		public AlertDialog.Builder setIcon(int iconId) {
			return this;
		}

		public AlertDialog.Builder setTitle(CharSequence title) {
			System.out.println("AlertDialog.Builder setTitle called with: '" + title + "'");
			dialog.setTitle(title);
			return this;
		}

		public AlertDialog.Builder setTitle(int title) {
			return setTitle(context.getResources().getText(title));
		}

		public AlertDialog.Builder setMessage(CharSequence message) {
			System.out.println("AlertDialog.Builder setMessage called with: '" + message + "'");
			dialog.setMessage(message);
			return this;
		}

		public AlertDialog.Builder setView(View view) {
			return this;
		}

		public AlertDialog.Builder setItems(CharSequence[] items, final DialogInterface.OnClickListener listener) {
			String[] stringItems = new String[items.length];
			for (int i = 0; i < items.length; i++) {
				stringItems[i] = String.valueOf(items[i]);
			}
			dialog.nativeSetItems(dialog.nativePtr, stringItems, listener);
			return this;
		}

		public Builder setOnCancelListener(OnCancelListener onCancelListener) {
			return this;
		}

		public AlertDialog create() {
			return dialog;
		}
	}
}
