package android.view;

public interface Menu {

	public MenuItem add(int groupId, int itemId, int order, CharSequence title);

	public MenuItem add(int groupId, int itemId,int order, int titleRes);

	public MenuItem findItem(int id);

	public MenuItem getItem(int id);

	public void clear();

	public void removeGroup(int groupId);

	public SubMenu addSubMenu(int id);

	public MenuItem add(int id);

	public MenuItem add(CharSequence text);

	public void setGroupCheckable(int group, boolean checkable, boolean exclusive);
}
