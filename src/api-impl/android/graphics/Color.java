package android.graphics;

public class Color {
	public static int argb(int alpha, int red, int green, int blue) {
		return (alpha << 24) | (red << 16) | (green << 8) | (blue << 0);
	}
}
