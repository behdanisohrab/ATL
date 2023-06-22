package android.graphics;

import android.content.res.AssetManager;

public class Typeface {

	/**
	 * The default NORMAL typeface object
	 */
	public static final Typeface DEFAULT = new Typeface();
	/**
	 * The default BOLD typeface object. Note: this may be not actually be
	 * bold, depending on what fonts are installed. Call getStyle() to know
	 * for sure.
	 */
	public static final Typeface DEFAULT_BOLD = new Typeface();
	/**
	 * The NORMAL style of the default sans serif typeface.
	 */
	public static final Typeface SANS_SERIF = new Typeface();
	/**
	 * The NORMAL style of the default serif typeface.
	 */
	public static final Typeface SERIF = new Typeface();
	/**
	 * The NORMAL style of the default monospace typeface.
	 */
	public static final Typeface MONOSPACE = new Typeface();

	// Style
	public static final int NORMAL = 0;
	public static final int BOLD = 1;
	public static final int ITALIC = 2;
	public static final int BOLD_ITALIC = 3;

	public static Typeface createFromAsset(AssetManager mgr, String path) {
		return null;
	}
}
