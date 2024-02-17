package android.widget;

import android.content.Context;
import android.graphics.drawable.Drawable;
import android.util.AttributeSet;
import android.view.View;

public class PopupWindow {

	public PopupWindow(Context context, AttributeSet attrs, int defStyleAttr, int defStyleRes) {
		popover = native_constructor();
	}

	private View contentView;
	private long popover;  // native pointer to GtkPopover

	public interface OnDismissListener {
		public void onDismiss();
	}

	protected native long native_constructor();
	protected native void native_setContentView(long widget, long contentView);
	protected native void native_showAsDropDown(long widget, long anchor, int xoff, int yoff, int gravity);

	public void setBackgroundDrawable(Drawable background) {}

	public void setInputMethodMode(int mode) {}

	public boolean isShowing() {return false;}

	public native void setOnDismissListener(OnDismissListener listener);

	public void setFocusable(boolean focusable) {}

	public Drawable getBackground() {return null;}

	public void setContentView(View view) {
		contentView = view;
		native_setContentView(popover, view.widget);
	}

	public int getInputMethodMode() {return 0;}

	public int getMaxAvailableHeight(View anchor, int yOffset) {return 500;}

	public native void setWidth(int width);

	public native void setHeight(int height);

	public void setOutsideTouchable(boolean touchable) {}

	public void setTouchInterceptor(View.OnTouchListener listener) {}

	public void showAsDropDown(View anchor, int xoff, int yoff, int gravity) {
		native_showAsDropDown(popover, anchor.widget, xoff, yoff, gravity);
	}

	public View getContentView() {return contentView;}
}
