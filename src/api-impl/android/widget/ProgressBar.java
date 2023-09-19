package android.widget;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.drawable.Drawable;
import android.util.AttributeSet;
import android.view.View;

public class ProgressBar extends View {
	public ProgressBar(Context context, AttributeSet attrs) {
		super(context, attrs);
	}

	public ProgressBar(Context context) {
		super(context);
	}

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
}
