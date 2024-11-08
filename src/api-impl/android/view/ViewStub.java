package android.view;

import android.content.Context;
import android.content.res.TypedArray;
import android.util.AttributeSet;

public class ViewStub extends View {

	private int layoutResource;
	private int inflatedId = View.NO_ID;

	public ViewStub(Context context) {
		this(context, null, 0);
	}

	public ViewStub(Context context, AttributeSet attributeSet) {
		this(context, attributeSet, 0);
	}

	public ViewStub(Context context, AttributeSet attributeSet, int defStyle) {
		super(context, attributeSet, defStyle);
		TypedArray a = context.obtainStyledAttributes(attributeSet, com.android.internal.R.styleable.ViewStub);
		layoutResource = a.getResourceId(com.android.internal.R.styleable.ViewStub_layout, -1);
		a.recycle();
	}

	public View inflate() {
		if (layoutResource == -1)
			throw new IllegalStateException("ViewStub must have a valid layoutResource");
		ViewGroup parent = (ViewGroup) getParent();
		View view = LayoutInflater.from(getContext()).inflate(layoutResource, parent, false);
		if (inflatedId != View.NO_ID)
			view.setId(inflatedId);
		int index = parent.indexOfChild(this);
		parent.removeView(this);
		parent.addView(view, index, getLayoutParams());
		return view;
	}

	public void setLayoutResource(int layoutResource) {
		this.layoutResource = layoutResource;
	}

	@Override
	protected void onMeasure(int widthMeasureSpec, int heightMeasureSpec) {
		setMeasuredDimension(0, 0);
	}

	public void setInflatedId(int inflatedId) {
		this.inflatedId = inflatedId;
	}

}
