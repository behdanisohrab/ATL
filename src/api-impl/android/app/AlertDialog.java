package android.app;

import android.content.Context;
import android.content.DialogInterface;
import android.view.View;

public class AlertDialog extends Dialog implements DialogInterface {

	private native void nativeSetMessage(long ptr, String message);
	private native void nativeSetButton(long ptr, int whichButton, String text);
	private native void nativeSetItems(long ptr, String[] items, DialogInterface.OnClickListener listener);

	public AlertDialog(Context context) {
		super(context, 0);
	}

	public AlertDialog(Context context, int themeResId) {
		super(context, themeResId);
	}

	public void setMessage(CharSequence message) {
		System.out.println("AlertDialog setMessage called with: '" + message + "'");
		nativeSetMessage(nativePtr, String.valueOf(message));
	}

	public void setButton(int whichButton, CharSequence text, OnClickListener listener) {
		nativeSetButton(nativePtr, whichButton, String.valueOf(text));
	}

	public static class Builder {
		private AlertDialog dialog;

		public Builder(Context context) {
			System.out.println("making an AlertDialog$Builder as we speak, my word!");
			dialog = new AlertDialog(context);
		}

		public AlertDialog.Builder setPositiveButton(int textId, DialogInterface.OnClickListener listener) {
			return setPositiveButton(dialog.getContext().getText(textId), listener);
		}

		public AlertDialog.Builder setPositiveButton(CharSequence text, DialogInterface.OnClickListener listener) {
			System.out.println("AlertDialog.Builder setPositiveButton called with text: '" + text + "'");
			dialog.setButton(DialogInterface.BUTTON_POSITIVE, text, listener);
			return this;
		}

		public AlertDialog.Builder setNegativeButton (CharSequence text, DialogInterface.OnClickListener listener) {
			System.out.println("AlertDialog.Builder setNegativeButton called with text: '" + text + "'");
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
			return setTitle(dialog.getContext().getText(title));
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
