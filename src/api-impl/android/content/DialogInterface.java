package android.content;

public interface DialogInterface {
	/** The identifier for the positive button. */
	int BUTTON_POSITIVE = -1;
	/** The identifier for the negative button. */
	int BUTTON_NEGATIVE = -2;
	/** The identifier for the neutral button. */
	int BUTTON_NEUTRAL = -3;

	public interface OnDismissListener {
	}
	public interface OnClickListener {
		void onClick(DialogInterface dialog, int which);
	}
	public interface OnShowListener {
	}
	public interface OnCancelListener {
	}
}
