package android.text;

import android.graphics.Canvas;

public class StaticLayout extends Layout {

	private CharSequence text;

	public StaticLayout(CharSequence source, int bufstart, int bufend,
						TextPaint paint, int outerwidth,
						Alignment align, TextDirectionHeuristic textDir,
						float spacingmult, float spacingadd,
						boolean includepad,
						TextUtils.TruncateAt ellipsize, int ellipsizedWidth, int maxLines) {
		this.text = source;
	}

	public CharSequence getText() {
		return text;
	}

	public int getWidth() {
		return 200;  // arbitrary value for stub method
	}

	public int getHeight() {
		return 50;  // arbitrary value for stub method
	}

	public float getLineLeft(int line) {return 0;}

	public void draw(Canvas canvas) {}
}
