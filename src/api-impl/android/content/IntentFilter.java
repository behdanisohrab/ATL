package android.content;

import java.util.HashSet;
import java.util.Set;

public class IntentFilter {
	
	private Set<String> actions = new HashSet<>();

	public IntentFilter() {}
	public IntentFilter(String action) {
		addAction(action);
	}

	public void addAction(String action) {
		actions.add(action);
	}
	public int countActions() {
		return actions.size();
	}

	public final boolean matchAction(String action) {
		return actions.contains(action);
	}
}
