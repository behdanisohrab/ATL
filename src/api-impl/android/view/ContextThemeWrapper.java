package android.view;

import android.content.Context;
import android.content.ContextWrapper;
import android.content.res.Resources;

public class ContextThemeWrapper extends ContextWrapper {

	private Resources.Theme theme = getResources().newTheme();

	public ContextThemeWrapper(Context context, int themeResId) {
		super(context);
		theme.setTo(context.getTheme());
		setTheme(themeResId);
	}

	@Override
	public void setTheme(int resid) {
		theme.applyStyle(resid, true);
	}

	@Override
	public Resources.Theme getTheme() {
		return theme;
	}

}
