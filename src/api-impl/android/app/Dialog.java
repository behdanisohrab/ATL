package android.app;

import android.content.Context;
import android.content.DialogInterface.OnCancelListener;
import android.content.DialogInterface.OnDismissListener;
import android.view.View;

public class Dialog {

	private Context context;

	public Dialog(Context context, int themeResId) {
		this.context = context;
	}

	public final boolean requestWindowFeature(int featureId) {
		return false;
	}

	public Context getContext() {
		return context;
	}

	public void setContentView(View view) {}

	public void setOwnerActivity(Activity activity) {}

	public void setCancelable(boolean cancelable) {}

	public void setOnCancelListener(OnCancelListener onCancelListener) {}

	public void setOnDismissListener(OnDismissListener onDismissListener) {}

	public void show() {
		System.out.println("totally showing the Dialog " + this + " right now, most definitely doing that");
	}

	public void dismiss() {
		System.out.println("totally dismissing the Dialog " + this + " right now, which was most definitely being shown just a moment ago");
	}

	public class Builder {
		public Builder(Context context) {
			System.out.println("making a Dialog$Builder as we speak, my word!");
		}
	}
}
