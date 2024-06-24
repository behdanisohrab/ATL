package android.view.inputmethod;

import android.os.IBinder;
import android.view.View;

public class InputMethodManager {

	public boolean hideSoftInputFromWindow(IBinder windowToken, int flags) {return false;}

	public boolean showSoftInput(View view, int flags) {return false;}

	public boolean isFullscreenMode() {return false;}

	public boolean isActive(View view) {return false;}

}
