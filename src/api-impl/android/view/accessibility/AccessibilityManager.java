package android.view.accessibility;

import java.util.ArrayList;
import java.util.List;

public class AccessibilityManager {

	public interface AccessibilityStateChangeListener {}

	public boolean isTouchExplorationEnabled() {return false;}

	public boolean isEnabled() {return false;}

	public List getEnabledAccessibilityServiceList(int feedbackTypeFlags) {
		return new ArrayList<>();
	}

	public boolean addAccessibilityStateChangeListener(AccessibilityStateChangeListener listener) {
		return false;
	}

}
