package android.app;

public class FragmentManager {
	
	public Fragment findFragmentByTag(String tag) {
		return null;
	}

	public FragmentTransaction beginTransaction() {
		return new FragmentTransaction();
	}

	public boolean executePendingTransactions() {
		return false;
	}
}
