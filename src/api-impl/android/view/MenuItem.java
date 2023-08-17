package android.view;

public interface MenuItem {

	public interface OnMenuItemClickListener {}

	public MenuItem setIcon(int iconRes);

	public MenuItem setVisible(boolean visible);

}