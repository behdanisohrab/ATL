package android.view;

import android.content.Context;
import android.content.ContextWrapper;

public class ContextThemeWrapper extends ContextWrapper {

	public ContextThemeWrapper(Context context, int themeResId) {
		super(context);
	}

}
