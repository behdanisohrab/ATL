package android.widget;

import android.content.Context;
import android.content.res.TypedArray;
import android.graphics.Canvas;
import android.graphics.drawable.Drawable;
import android.util.AttributeSet;
import android.view.View;

public class ProgressBar extends View {

	protected int max = 100;
	protected int progress = 0;
	private Drawable indeterminateDrawable;

	public ProgressBar(Context context, AttributeSet attrs, int defStyle) {
		super(context, attrs, defStyle);
		haveCustomMeasure = false;
		TypedArray a = context.obtainStyledAttributes(attrs, com.android.internal.R.styleable.ProgressBar, defStyle, 0);
		setIndeterminate(a.getBoolean(com.android.internal.R.styleable.ProgressBar_indeterminate, false));
		a.recycle();
	}

	public ProgressBar(Context context, AttributeSet attrs) {
		this(context, attrs, 0);
	}

	public ProgressBar(Context context) {
		this(context, null, 0);
	}

	@Override
	protected native long native_constructor(Context context, AttributeSet attrs);
	protected native void native_setProgress(long widget, float fraction);

	public native void setIndeterminate(boolean indeterminate);

	public Drawable getProgressDrawable() {
		return new Drawable() {
			@Override
			public void draw(Canvas canvas) {
				// TODO Auto-generated method stub
				throw new UnsupportedOperationException("Unimplemented method 'draw'");
			}
		};
	}

	public Drawable getIndeterminateDrawable() {
		return indeterminateDrawable;
	}

	public void setMax(int max) {
		this.max = max;
		native_setProgress(widget, progress / (float)max);
	}

	public int getMax() {
		return max;
	}

	public void setProgress(int progress) {
		this.progress = progress;
		native_setProgress(widget, progress / (float)max);
	}

	public void setSecondaryProgress(int secondaryProgress) {}

	public int getProgress() {
		return progress;
	}

	public void setIndeterminateDrawable(Drawable indeterminateDrawable) {
		this.indeterminateDrawable = indeterminateDrawable;
	}
}
