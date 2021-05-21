package android.view;

import android.util.AttributeSet;

public class LinearLayout extends ViewGroup {
		public LinearLayout(AttributeSet _attrs) {
		attrs = _attrs;

		System.out.println("creating LinearLayout with the attrs:"+attrs+"; "+attrs.getAttributeName(0));
	}
}
