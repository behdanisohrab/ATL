package android.text;

import android.graphics.Canvas;

public class Layout {
	
	public enum Alignment {
		ALIGN_NORMAL,
		ALIGN_OPPOSITE,
		ALIGN_CENTER,
		ALIGN_LEFT,
		ALIGN_RIGHT,
	}

	public int getLineCount() {return 1;}

	public float getLineWidth(int line) {return 10;}

	public TextPaint getPaint() {return new TextPaint();}

	public int getEllipsisCount(int line) {return 0;}

	public CharSequence getText() {return "FIXME Layout.getText";}

	public int getWidth() {return 10;}

	public int getHeight() {return 10;}

	public void draw(Canvas canvas) {}

	public int getParagraphDirection(int line) {return 0;}
}
