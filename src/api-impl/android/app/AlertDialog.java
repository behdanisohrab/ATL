package android.app;

import android.content.Context;
import android.content.DialogInterface;
import android.view.View;

public class AlertDialog extends Dialog {
	public static class Builder {
		public Builder(Context context){
			System.out.println("making an AlertDialog$Builder as we speak, my word!");
		}

		public AlertDialog.Builder setPositiveButton (int textId, DialogInterface.OnClickListener listener) {
			return this;
		}

		public AlertDialog.Builder setPositiveButton (CharSequence text, DialogInterface.OnClickListener listener) {
			return this;
		}

		public AlertDialog.Builder setCancelable (boolean cancelable) {
			return this;
		}

		public AlertDialog.Builder setIcon (int iconId) {
			return this;
		}

		public AlertDialog.Builder setTitle (CharSequence title) {
			System.out.println("AlertDialog.Builder setTitle called with: '"+title+"'");
			return this;
		}

		public AlertDialog.Builder setMessage (CharSequence message) {
			System.out.println("AlertDialog.Builder setMessage called with: '"+message+"'");
			return this;
		}

		public AlertDialog.Builder setView (View view) {
			return this;
		}

		public AlertDialog create() {
			return new AlertDialog();
		}
	}
}
