package android.view;

public class WindowInsets {

	public static final WindowInsets CONSUMED = new WindowInsets();

	public WindowInsets() {}

	public WindowInsets(WindowInsets windowInsets) {}

	public WindowInsets consumeStableInsets() {
		return this;
	}

	public WindowInsets consumeSystemWindowInsets() {
		return this;
	}
}
