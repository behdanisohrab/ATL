package android.view;

import android.util.AttributeSet;

public class LinearLayout extends ViewGroup {

	boolean orientation;

	public LinearLayout(AttributeSet attrs) {
		super(attrs);

		native_constructor(attrs);
	}

	public native void native_constructor(AttributeSet attrs); /*{
		int _orientation = attrs.getAttributeIntValue("http://schemas.android.com/apk/res/android", "orientation", 0);

		if (_orientation == 0)
			orientation = Orientation.HORIZONTAL;
		else
			orientation = Orientation.VERTICAL;

		widget = (Container)(new Box(orientation, 1)); // spacing 1

		System.out.println("[[~~~~~~~~~~~");
		System.out.println(this);
		System.out.println(this.widget);
		System.out.println("[[~~~~~~~~~~~");

		System.out.println("creating LinearLayout with the attrs:"+attrs+"; "+attrs.getAttributeName(0));
	}*/

	@Override
	public native void addView(View child, int index, LayoutParams params);
}
