package android.graphics;

/**
 * GskCanvas:
 *   - implements Canvas for onscreen rendering inside GTKs snapshot function
 */
public class GskCanvas extends Canvas {
	private long snapshot;

	public GskCanvas(long snapshot) {
		System.out.println("GskCanvas(" + snapshot + ")");
		this.snapshot = snapshot;
	}

	@Override
	public int save() {
		System.out.println("GskCanvas.save()");
		return -1;
	}

	@Override
	public void restore() {
		System.out.println("GskCanvas.restore()");
	}

	@Override
	public void translate(float dx, float dy) {
		System.out.println("GskCanvas.translate(" + dx + ", " + dy + ")");
	}

	@Override
	public void rotate(float degrees) {
		System.out.println("GskCanvas.rotate(" + degrees + ")");
	}

	@Override
	public void drawBitmap(Bitmap bitmap, Rect src, Rect dst, Paint paint) {
		int color = 0;
		if (paint != null && paint.colorFilter instanceof PorterDuffColorFilter) {
			color = ((PorterDuffColorFilter) paint.colorFilter).getColor();
		}
		native_drawBitmap(snapshot, bitmap.pixbuf, dst.left, dst.top, dst.width(), dst.height(), color);
	}

	@Override
	public void drawPath(Path path, Paint paint) {
		System.out.println("GskCanvas.drawPath(" + path + ", " + paint + ")");
	}

	@Override
	public void drawRect(float left, float top, float right, float bottom, Paint paint) {
		native_drawRect(snapshot, left, top, right, bottom, paint.getColor());
	}

	@Override
	public void rotate(float degrees, float px, float py) {
		System.out.println("GskCanvas.rotate(" + degrees + ", " + px + ", " + py + ")");
	}

	@Override
	public void drawText(String text, float x, float y, Paint paint) {
		System.out.println("GskCanvas.drawText(" + text + ", " + x + ", " + y + ", " + paint + ")");
	}

	@Override
	public void drawLine(float startX, float startY, float stopX, float stopY, Paint paint) {
		System.out.println("GskCanvas.drawLine(" + startX + ", " + startY + ", " + stopX + ", " + stopY + ", " + paint + ")");
	}

	@Override
	public void drawBitmap(Bitmap bitmap, float left, float top, Paint paint) {
		Rect src = new Rect(0, 0, bitmap.getWidth(), bitmap.getHeight());
		Rect dst = new Rect((int)left, (int)top, (int)left + bitmap.getWidth(), (int)top + bitmap.getHeight());
		drawBitmap(bitmap, src, dst, paint);
	}

	protected native void native_drawBitmap(long snapshot, long pixbuf, int x, int y, int width, int height, int color);
	protected native void native_drawRect(long snapshot, float left, float top, float right, float bottom, int color);
}
