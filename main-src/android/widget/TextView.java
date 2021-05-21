package android.widget;

import android.util.AttributeSet;

import android.view.View;

public class TextView extends View {
	public String text;

	public TextView(int _id) { // FIXME
		id = _id;
	}

	public TextView(AttributeSet attrs) {
		super(attrs);

		text = attrs.getAttributeValue("http://schemas.android.com/apk/res/android", "text");

		System.out.println("creating TextView with the attrs:"+attrs+"; id:"+id+", text:"+text+";;"+attrs.getAttributeNamespace(0)+";;");
	}

	public final void setText(CharSequence _text) {
		System.out.println("setting TextView with id " + id + " to '" + _text + "'");

		text = (String)_text;
	}
}
