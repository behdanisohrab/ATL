package android.view;

public interface Menu {

	public MenuItem add(int groupId, int itemId, int order, CharSequence title);

	public MenuItem add(int groupId, int itemId,int order, int titleRes);

	public MenuItem findItem(int id);

	public void clear();
}
