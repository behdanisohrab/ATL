package android.graphics.drawable;

import android.graphics.Canvas;
import android.graphics.Paint;
import android.graphics.drawable.shapes.Shape;

public class ShapeDrawable extends Drawable {

	public ShapeDrawable(Shape shape) {}

	public Paint getPaint() {return new Paint();}

	@Override
	public void draw(Canvas canvas) {
		// TODO Auto-generated method stub
		throw new UnsupportedOperationException("Unimplemented method 'draw'");
	}
	
}
