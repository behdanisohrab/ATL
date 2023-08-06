package android.view;

import android.content.Context;
import android.content.res.XmlResourceParser;
import android.util.AttributeSet;
import android.util.Xml;
import java.io.FileReader;
import java.lang.reflect.Constructor;
import org.xmlpull.v1.XmlPullParser;
import org.xmlpull.v1.XmlPullParserException;
import org.xmlpull.v1.XmlPullParserFactory;

public class LayoutInflater {

	public interface Factory {
	}

	public interface Factory2 {
	}

	private Factory2 factory2;

	public final LayoutInflater.Factory getFactory() {
		return null;
	}

	public void setFactory2(Factory2 factory) {
		this.factory2 = factory;
	}

	public static LayoutInflater from(Context context) {
		return new LayoutInflater();
	}

	public final View createView(String name, String prefix, AttributeSet attrs) throws Exception {
		System.out.println(">>>>>>>>>>>>>>>>>>>>>>>>> createView(" + name + ", " + prefix + ", " + attrs + ");");

		String view_class_name = prefix!=null ? prefix + name : name;
		Class view_class = Class.forName(view_class_name);

		Constructor constructor = view_class.getConstructor(Context.class, AttributeSet.class);

		View view_instance = (View)constructor.newInstance(Context.this_application, attrs);

		return view_instance;
	}

	/**
	 * taken as-is
	 */
	protected View onCreateView(String name, AttributeSet attrs) throws Exception {
		try { // FIXME ugly
			return createView(name, "android.view.", attrs);
		} catch (java.lang.ClassNotFoundException e) {
			return createView(name, "android.widget.", attrs);
		}
	}

	/**
	 * taken as-is
	 */
	protected View onCreateView(View parent, String name, AttributeSet attrs) throws Exception {
		return onCreateView(name, attrs);
	}

	View createViewFromTag(View parent, String name, AttributeSet attrs) throws Exception {
		if (name.equals("view")) {
			name = attrs.getAttributeValue(null, "class");
		}

		System.out.println("******** Creating view: " + name);

		View view;

		if (-1 == name.indexOf('.')) {
			view = onCreateView(parent, name, attrs);
		} else {
			view = createView(name, null, attrs);
		}

		System.out.println("Created view is: " + view);
		return view;
	}

	public View inflate(int resource, ViewGroup root) throws Exception {
		return inflate(resource, root, root != null);
	}

	public View inflate(int layoutResID, ViewGroup root, boolean attachToRoot) throws Exception {

		XmlResourceParser xpp = Context.this_application.getResources().getLayout(layoutResID);

		return inflate(xpp, root, attachToRoot);
	}

	public View inflate(XmlPullParser parser, ViewGroup root, boolean attachToRoot) throws Exception {

		final AttributeSet attrs = Xml.asAttributeSet(parser);

		View result = root;

		// Look for the root node.
		int type;
		while ((type = parser.next()) != XmlPullParser.START_TAG &&
		       type != XmlPullParser.END_DOCUMENT) {
			// Empty
		}

		if (type != XmlPullParser.START_TAG) {
			throw new Exception(parser.getPositionDescription() + ": No start tag found!");
		}

		final String name = parser.getName();

		System.out.println("**************************");
		System.out.println("Creating root view: " + name);
		System.out.println("**************************");

		if (name.equals("merge")) {
			if (root == null || !attachToRoot) {
				throw new Exception("<merge /> can be used only with a valid ViewGroup root and attachToRoot=true");
			}

			rInflate(parser, root, attrs, false);
		} else {
			// Temp is the root view that was found in the xml
			View temp;
			if (name.equals("blink")) {
				throw new Exception("<blink> not supported atm");
				/*temp = new BlinkLayout(mContext, attrs);*/
			} else {
				temp = createViewFromTag(root, name, attrs);
			}

			ViewGroup.LayoutParams params = null;

			if (root != null) {
				System.out.println("Creating params from root: " + root);

				// Create layout params that match root, if supplied
				params = root.generateLayoutParams(attrs);
				if (!attachToRoot) {
					// Set the layout params for temp if we are not
					// attaching. (If we are, we use addView, below)
					temp.setLayoutParams(params);
				}
			}

			System.out.println("-----> start inflating children");

			// Inflate all children under temp
			rInflate(parser, temp, attrs, true);

			System.out.println("-----> done inflating children");

			// We are supposed to attach all the views we found (int temp)
			// to root. Do that now.
			if (root != null && attachToRoot) {
				root.addView(temp, params);
			}

			// Decide whether to return the root that was passed in or the
			// top view found in xml.
			if (root == null || !attachToRoot) {
				result = temp;
			}
		}

		return result;
	}

	void rInflate(XmlPullParser parser, View parent, final AttributeSet attrs, boolean finishInflate) throws Exception {

		final int depth = parser.getDepth();
		int type;

		while (((type = parser.next()) != XmlPullParser.END_TAG ||
			parser.getDepth() > depth) &&
		       type != XmlPullParser.END_DOCUMENT) {

			if (type != XmlPullParser.START_TAG) {
				continue;
			}

			final String name = parser.getName();

			if (name.equals("requestFocus")) {
				throw new Exception("<requestFocus /> not supported atm");
				// parseRequestFocus(parser, parent);
			} else if (name.equals("include")) {
				if (parser.getDepth() == 0) {
					throw new Exception("<include /> cannot be the root element");
				}
				parseInclude(parser, parent, attrs);
			} else if (name.equals("merge")) {
				throw new Exception("<merge /> must be the root element");
			} else if (name.equals("blink")) {
				throw new Exception("<blink> not supported atm");
				/*final View view = new BlinkLayout(mContext, attrs);
				final ViewGroup viewGroup = (ViewGroup) parent;
				final ViewGroup.LayoutParams params = viewGroup.generateLayoutParams(attrs);
				rInflate(parser, view, attrs, true);
				viewGroup.addView(view, params);*/
			} else {
				final View view = createViewFromTag(parent, name, attrs);
				final ViewGroup viewGroup = (ViewGroup)parent;
				final ViewGroup.LayoutParams params = viewGroup.generateLayoutParams(attrs);
				rInflate(parser, view, attrs, true);
				viewGroup.addView(view, params);
			}
		}

		if (finishInflate)
			parent.onFinishInflate();
	}

	private void parseInclude(XmlPullParser parser, View parent, AttributeSet attrs) throws Exception {
		int layout = attrs.getAttributeResourceValue(null, "layout", 0);
		View view = inflate(layout, (ViewGroup)parent, true);
		if (view == null)
			return;

		int id = attrs.getAttributeResourceValue("http://schemas.android.com/apk/res/android", "id", 0);
		if (id != 0)
			view.setId(id);
	}
}
