package android.content;

import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;
import java.util.Set;

public class IntentFilter {
	
	private List<String> actions = new ArrayList<>();
	private Set<String> categories = new HashSet<>();

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

	public void addCategory(String category) {
		categories.add(category);
	}
	public int countCategories() {
		return categories.size();
	}

	public final boolean hasCategory(String category) {
		return categories.contains(category);
	}

	public String getAction(int index) {
		return actions.get(index);
	}

	public void setPriority(int priority) {}
}
