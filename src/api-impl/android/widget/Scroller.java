package android.widget;

import android.content.Context;
import android.util.AttributeSet;
import android.view.View;
import android.view.animation.Interpolator;

public class Scroller extends View {

	public Scroller(Context context) {
		super(context);
	}

	public Scroller(Context context, AttributeSet attributeSet) {
		super(context, attributeSet);
	}

	public Scroller(Context context, Interpolator interpolator) {
		this(context);
	}

}
