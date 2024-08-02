package android.widget;

import android.content.Context;
import android.util.AttributeSet;

public class ImageButton extends ImageView {

	public ImageButton(Context context) {
		super(context);
	}

	public ImageButton(Context context, AttributeSet attributeSet) {
		super(context, attributeSet);
	}

	public ImageButton(Context context, AttributeSet attributeSet, int defStyleAttr) {
		super(context, attributeSet, defStyleAttr);
	}

	@Override
	protected native long native_constructor(Context context, AttributeSet attrs);
	@Override
	protected native void native_setPixbuf(long widget, long pixbuf);
	@Override
	protected native void native_setDrawable(long widget, long paintable);
	@Override
	protected native void nativeSetOnClickListener(long widget);

}
