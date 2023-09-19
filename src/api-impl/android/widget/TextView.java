package android.widget;

import android.content.Context;
import android.content.res.ColorStateList;
import android.content.res.TypedArray;
import android.graphics.Typeface;
import android.graphics.drawable.Drawable;
import android.text.Editable;
import android.text.InputFilter;
import android.text.TextPaint;
import android.text.TextUtils;
import android.text.TextWatcher;
import android.text.method.KeyListener;
import android.text.method.MovementMethod;
import android.text.method.TransformationMethod;
import android.text.style.URLSpan;
import android.util.AttributeSet;
import android.view.Gravity;
import android.view.View;

public class TextView extends View {
	public String text;

	public TextView(int _id) { // FIXME
		id = _id;
	}

	public TextView(Context context, AttributeSet attrs) {
		this(context, attrs, 0);
	}

	public TextView(Context context) {
		this(context, null);
	}

	public TextView(Context context, AttributeSet attrs, int defStyleAttr) {
		super(context, attrs, defStyleAttr);

		TypedArray a = context.obtainStyledAttributes(attrs, com.android.internal.R.styleable.TextView, defStyleAttr, 0);
		if (a.hasValue(com.android.internal.R.styleable.TextView_text)) {
			setText(a.getText(com.android.internal.R.styleable.TextView_text));
		}
		int ap = a.getResourceId(com.android.internal.R.styleable.TextView_textAppearance, -1);
		a.recycle();
		if (ap != -1) {
			a = context.obtainStyledAttributes(ap, com.android.internal.R.styleable.TextAppearance);
			if (a.hasValue(com.android.internal.R.styleable.TextAppearance_textSize)) {
				setTextSize(a.getDimensionPixelSize(com.android.internal.R.styleable.TextAppearance_textSize, 10));
			}
			a.recycle();
		}
	}

	@Override
	protected native long native_constructor(Context context, AttributeSet attrs);

	public void setText(CharSequence text) {
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

	@Override
	protected void onMeasure(int widthMeasureSpec, int heightMeasureSpec) {
		native_measure(widget, widthMeasureSpec, heightMeasureSpec, true);
	}

	private native final void native_set_markup(int bool);

	public native final void native_setText(String text);
	public native void setTextSize(float size);

	public native final void native_setTextColor(int color);
	public void setTextColor(int color) {
		native_setTextColor(color);
	}
	public void setTextColor(ColorStateList colors) {
		if (colors != null)
			setTextColor(colors.getDefaultColor()); // TODO: do this properly
	}
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
	public void removeTextChangedListener(TextWatcher watcher) {}
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

	public Typeface getTypeface() {return null;}

	public float getTextSize() {return 10;}

	public int getGravity() {
		return Gravity.CENTER;
	}

	public int getCompoundPaddingTop() {return 0;}
	public int getCompoundPaddingBottom() {return 0;}

	public CharSequence getText() {
		return "FIXME";
	}

	public void setCompoundDrawablePadding(int pad) {}

	public void setCompoundDrawables(Drawable left, Drawable top, Drawable right, Drawable bottom) {}

	public void setAllCaps(boolean allCaps) {}

	public void setSaveEnabled(boolean enabled) {}

	public final void setAutoLinkMask(int mask) {}

	public void setEditableFactory(Editable.Factory factory) {}

	public KeyListener getKeyListener() {return null;}

	public int getInputType() {return 0;}

	public final void setTransformationMethod(TransformationMethod method) {}

	public InputFilter[] getFilters() {return new InputFilter[0];}

	public int getMaxLines() {return -1;}

	public void setCompoundDrawablesRelative(Drawable start, Drawable top, Drawable end, Drawable bottom) {}

	public int getLineCount() {return 1;}

	public URLSpan[] getUrls() {return new URLSpan[0];}

	public void setMovementMethod(MovementMethod method) {}

	public void setTextIsSelectable(boolean selectable) {}

	public static interface OnEditorActionListener {
	}

	public static enum BufferType {
		EDITABLE,
		NORMAL,
		SPANNABLE,
	}
}
