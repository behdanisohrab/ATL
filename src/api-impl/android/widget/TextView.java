package android.widget;

import android.content.Context;
import android.content.res.ColorStateList;
import android.content.res.TypedArray;
import android.graphics.Typeface;
import android.text.InputFilter;
import android.text.TextPaint;
import android.text.TextUtils;
import android.text.TextWatcher;
import android.text.method.TransformationMethod;
import android.util.AttributeSet;
import android.view.View;

public class TextView extends View {
	public String text;

	public TextView(int _id) { // FIXME
		id = _id;
	}

	public TextView(Context context, AttributeSet attrs) {
		super(context, attrs);
	}

	public TextView(Context context) {
		super(context);
	}

	public TextView(Context context, AttributeSet attrs, int defStyleAttr) {
		this(context, attrs);
	}

	@Override
	protected native long native_constructor(Context context, AttributeSet attrs);

	public final void setText(CharSequence text) {
		if (text == null) {
			native_setText("NULL");
			return;
		}

		native_setText(text.toString());

		if (text instanceof android.text.Spanned)
			native_set_markup(1);
	}

	public void setText(int resId) {
		setText(getContext().getResources().getText(resId));
	}

	private native final void native_set_markup(int bool);

	public native final void native_setText(String text);
	public native void setTextSize(float size);

	public void setTextColor(int color) {}
	public void setTextColor(ColorStateList colors) {}
	public void setTextSize(int unit, float size) {}
	public void setTypeface(Typeface tf, int style) {}
	public void setTypeface(Typeface tf) {}
	public void setLineSpacing(float add, float mult) {}
	public final void setLinksClickable(boolean whether) {}

	public void setInputType(int type) {}
	public void setFilters(InputFilter[] filters) {}
	public void setCursorVisible(boolean visible) {}
	public void setImeOptions(int imeOptions) {}

	public final ColorStateList getTextColors() { return new ColorStateList(new int[][] {new int[0]}, new int[1]); }
	public static ColorStateList getTextColors(Context context, TypedArray attrs) { return new ColorStateList(new int[][] {new int[0]}, new int[1]); }

	public TextPaint getPaint() {
		return new TextPaint();
	}

	public void addTextChangedListener(TextWatcher watcher) {}
	public void setOnEditorActionListener(TextView.OnEditorActionListener l) {}

	public TransformationMethod getTransformationMethod() {
		return null;
	}

	public void setHintTextColor(ColorStateList colorStateList) {}
	public void setLinkTextColor(ColorStateList colorStateList) {}

	public void setSingleLine() {}

	public void setEllipsize(TextUtils.TruncateAt truncateAt) {}

	public void setTextAppearance(Context context, int appearance) {}

	public void setMaxLines(int maxLines) {}

	public void setMinWidth(int minWidth) {}
	public void setMaxWidth(int maxWidth) {}

	public static interface OnEditorActionListener {
	}
}
