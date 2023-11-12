package android.widget;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.drawable.Drawable;
import android.util.AttributeSet;
import android.view.View;

public class ProgressBar extends View {

	private int max = 100;
	private int progress = 0;

	public ProgressBar(Context context, AttributeSet attrs) {
		super(context, attrs);
	}

	public ProgressBar(Context context) {
		super(context);
	}

	@Override
	protected native long native_constructor(Context context, AttributeSet attrs);
	private native void native_setProgress(long widget, float fraction);

	public synchronized void setIndeterminate(boolean indeterminate) {}

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
		return new Drawable() {
			@Override
			public void draw(Canvas canvas) {
				// TODO Auto-generated method stub
				throw new UnsupportedOperationException("Unimplemented method 'draw'");
			}
		};
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
}
