package android.graphics.drawable;

import android.graphics.Paint;
import android.graphics.drawable.shapes.Shape;

public class ShapeDrawable extends Drawable {

	public ShapeDrawable(Shape shape) {}

	public Paint getPaint() {return new Paint();}
	
}
