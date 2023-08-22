package android.widget;

import android.content.Context;
import android.text.Editable;
import android.util.AttributeSet;

public class EditText extends TextView {
	public EditText(Context context) {
		super(context);
	}

	public EditText(Context context, AttributeSet attrs) {
		super(context, attrs);
	}

	@Override
	protected native long native_constructor(Context context, AttributeSet attrs);
	protected native String native_getText(long widget);

	public Editable getText() {
		return new FIXME_Editable(native_getText(widget));
	}

	public Editable getEditableText() {
		return new FIXME_Editable(native_getText(widget));
	}

	@Override
	public void setText(CharSequence text) {}
	@Override
	public void setTextSize(float size) {}

	private class FIXME_Editable implements Editable {
		private String string;

		public FIXME_Editable(String string) {
			this.string = string;
		}

		@Override
		public int length() {
			return string.length();
		}

		@Override
		public char charAt(int index) {
			return string.charAt(index);
		}

		@Override
		public CharSequence subSequence(int start, int end) {
			return string.subSequence(start, end);
		}

		@Override
		public String toString() {
			return string;
		}

	}
}
