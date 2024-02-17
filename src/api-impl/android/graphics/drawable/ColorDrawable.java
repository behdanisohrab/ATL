package android.graphics.drawable;

public class ColorDrawable extends Drawable {

	private int color;

	public ColorDrawable(int color) {
		this.color = color;
	}

	public int getColor() {
		return color;
	}

}
