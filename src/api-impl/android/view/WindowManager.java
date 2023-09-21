package android.view;

public interface WindowManager {
	public android.view.Display getDefaultDisplay();

	public class LayoutParams extends ViewGroup.LayoutParams {
		public static final int FLAG_KEEP_SCREEN_ON = 0;

		public float screenBrightness;
	}
}
