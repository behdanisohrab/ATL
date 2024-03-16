package android.view;

public interface WindowManager {
	public android.view.Display getDefaultDisplay();

	public class LayoutParams extends ViewGroup.LayoutParams {
		public static final int FLAG_KEEP_SCREEN_ON = 0;

		public float screenBrightness;
		public int softInputMode;
		public int x;
		public int y;

		public LayoutParams(int w, int h, int type, int flags, int format) {
			super(w, h);
		}

		public LayoutParams() {}
	}
}
