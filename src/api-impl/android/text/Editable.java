package android.text;

public interface Editable extends CharSequence {

	public class Factory {}

	public Editable replace(int start, int end, CharSequence source, int destoff, int destlen);

	public Editable replace(int start, int end, CharSequence text);

	public InputFilter[] getFilters();

	public void setFilters(InputFilter[] filters);

}
