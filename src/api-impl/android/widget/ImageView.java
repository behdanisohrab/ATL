package android.widget;

import android.content.Context;
import android.content.res.ColorStateList;
import android.content.res.TypedArray;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Matrix;
import android.graphics.PorterDuff;
import android.graphics.drawable.BitmapDrawable;
import android.graphics.drawable.Drawable;
import android.util.AttributeSet;
import android.view.View;

public class ImageView extends View {

	private Bitmap bitmap = null;
	private ScaleType scaleType = ScaleType.FIT_CENTER;

	public ImageView(Context context, AttributeSet attrs) {
		this(context, attrs, 0);
	}

	public ImageView(Context context) {
		this(context, null);
	}

	public ImageView(Context context, AttributeSet attrs, int defStyleAttr) {
		super(context, attrs, defStyleAttr);

		haveCustomMeasure = false;
		TypedArray a = context.obtainStyledAttributes(attrs, com.android.internal.R.styleable.ImageView, defStyleAttr, 0);
		setImageDrawable(a.getDrawable(com.android.internal.R.styleable.ImageView_src));
		a.recycle();
	}

	@Override
	protected native long native_constructor(Context context, AttributeSet attrs);
	protected native void native_setPixbuf(long pixbuf);
	protected native void native_setDrawable(long widget, long paintable);

	public /*native*/ void setImageResource(final int resid) {
		if (Context.this_application.getResources().getString(resid).endsWith(".xml")) {
			setImageDrawable(getResources().getDrawable(resid));
			return;
		}
		bitmap = BitmapFactory.decodeResource(Context.this_application.getResources(), resid);
		native_setPixbuf(bitmap.pixbuf);
	}
	public void setAdjustViewBounds(boolean adjustViewBounds) {}

	public void setScaleType(ScaleType scaleType) {
		this.scaleType = scaleType;
	}

	public ScaleType getScaleType() {
		return scaleType;
	}

	public Drawable getDrawable() {
		if(bitmap == null)
			return null;

		return new BitmapDrawable(getContext().getResources(), bitmap);
	}

	public void setImageDrawable(Drawable drawable) {
		if (drawable instanceof BitmapDrawable) {
			setImageBitmap(((BitmapDrawable) drawable).getBitmap());
		} else if (drawable != null && drawable.paintable != 0) {
			drawable.setCallback(this);
			native_setDrawable(widget, drawable.paintable);
		}
	}

	public void setImageMatrix(Matrix matrix) {}

	public void setImageBitmap(Bitmap bitmap) {
		if (bitmap != null)
			native_setPixbuf(bitmap.pixbuf);
	}

	/**
	 * Options for scaling the bounds of an image to the bounds of this view.
	 */
	public enum ScaleType {
		/**
		 * Scale using the image matrix when drawing. The image matrix can be set using
		 * {@link ImageView#setImageMatrix(Matrix)}. From XML, use this syntax:
		 * <code>android:scaleType="matrix"</code>.
		 */
		MATRIX(0),
		/**
		 * Scale the image using {@link Matrix.ScaleToFit#FILL}.
		 * From XML, use this syntax: <code>android:scaleType="fitXY"</code>.
		 */
		FIT_XY(1),
		/**
		 * Scale the image using {@link Matrix.ScaleToFit#START}.
		 * From XML, use this syntax: <code>android:scaleType="fitStart"</code>.
		 */
		FIT_START(2),
		/**
		 * Scale the image using {@link Matrix.ScaleToFit#CENTER}.
		 * From XML, use this syntax:
		 * <code>android:scaleType="fitCenter"</code>.
		 */
		FIT_CENTER(3),
		/**
		 * Scale the image using {@link Matrix.ScaleToFit#END}.
		 * From XML, use this syntax: <code>android:scaleType="fitEnd"</code>.
		 */
		FIT_END(4),
		/**
		 * Center the image in the view, but perform no scaling.
		 * From XML, use this syntax: <code>android:scaleType="center"</code>.
		 */
		CENTER(5),
		/**
		 * Scale the image uniformly (maintain the image's aspect ratio) so
		 * that both dimensions (width and height) of the image will be equal
		 * to or larger than the corresponding dimension of the view
		 * (minus padding). The image is then centered in the view.
		 * From XML, use this syntax: <code>android:scaleType="centerCrop"</code>.
		 */
		CENTER_CROP(6),
		/**
		 * Scale the image uniformly (maintain the image's aspect ratio) so
		 * that both dimensions (width and height) of the image will be equal
		 * to or less than the corresponding dimension of the view
		 * (minus padding). The image is then centered in the view.
		 * From XML, use this syntax: <code>android:scaleType="centerInside"</code>.
		 */
		CENTER_INSIDE(7);

		ScaleType(int ni) {
			nativeInt = ni;
		}
		final int nativeInt;
	}

	public final void setColorFilter(int color, PorterDuff.Mode mode) {}

	public void setImageTintList(ColorStateList tint) {}

	public void setImageAlpha(int alpha) {}

	public void setMaxWidth(int width) {}

	public void setMaxHeight(int height) {}

	public void setImageState(int[] state, boolean merge) {}
}
