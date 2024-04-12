package android.view.accessibility;

public class AccessibilityNodeInfo {
	public static final class AccessibilityAction {
		public static final AccessibilityNodeInfo.AccessibilityAction ACTION_SHOW_ON_SCREEN = new AccessibilityAction(0, null);
		public static final AccessibilityNodeInfo.AccessibilityAction ACTION_SCROLL_TO_POSITION = new AccessibilityAction(0, null);
		public static final AccessibilityNodeInfo.AccessibilityAction ACTION_SCROLL_UP = new AccessibilityAction(0, null);
		public static final AccessibilityNodeInfo.AccessibilityAction ACTION_SCROLL_LEFT = new AccessibilityAction(0, null);
		public static final AccessibilityNodeInfo.AccessibilityAction ACTION_SCROLL_RIGHT = new AccessibilityAction(0, null);
		public static final AccessibilityNodeInfo.AccessibilityAction ACTION_SCROLL_DOWN = new AccessibilityAction(0, null);
		public static final AccessibilityNodeInfo.AccessibilityAction ACTION_CONTEXT_CLICK = new AccessibilityAction(0, null);

		public AccessibilityAction(int actionId, CharSequence label) {}

		public int getId() {return 0;}
	}
}
