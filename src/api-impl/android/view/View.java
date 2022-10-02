package android.view;

import android.util.AttributeSet;
import android.util.LayoutDirection;
import android.content.Context;
import android.graphics.Rect;
import android.graphics.Canvas;

import java.util.HashMap;

public class View extends Object {

// --- constants from android source

   /**
     * The logging tag used by this class with android.util.Log.
     */
    protected static final String VIEW_LOG_TAG = "View";

    /**
     * When set to true, apps will draw debugging information about their layouts.
     *
     * @hide
     */
    public static final String DEBUG_LAYOUT_PROPERTY = "debug.layout";

    /**
     * Used to mark a View that has no ID.
     */
    public static final int NO_ID = -1;

    /**
     * Signals that compatibility booleans have been initialized according to
     * target SDK versions.
     */
    private static boolean sCompatibilityDone = false;

    /**
     * Use the old (broken) way of building MeasureSpecs.
     */
    private static boolean sUseBrokenMakeMeasureSpec = false;

    /**
     * Ignore any optimizations using the measure cache.
     */
    private static boolean sIgnoreMeasureCache = false;

    /**
     * This view does not want keystrokes. Use with TAKES_FOCUS_MASK when
     * calling setFlags.
     */
    private static final int NOT_FOCUSABLE = 0x00000000;

    /**
     * This view wants keystrokes. Use with TAKES_FOCUS_MASK when calling
     * setFlags.
     */
    private static final int FOCUSABLE = 0x00000001;

    /**
     * Mask for use with setFlags indicating bits used for focus.
     */
    private static final int FOCUSABLE_MASK = 0x00000001;

    /**
     * This view will adjust its padding to fit sytem windows (e.g. status bar)
     */
    private static final int FITS_SYSTEM_WINDOWS = 0x00000002;

    /**
     * This view is visible.
     * Use with {@link #setVisibility} and <a href="#attr_android:visibility">{@code
     * android:visibility}.
     */
    public static final int VISIBLE = 0x00000000;

    /**
     * This view is invisible, but it still takes up space for layout purposes.
     * Use with {@link #setVisibility} and <a href="#attr_android:visibility">{@code
     * android:visibility}.
     */
    public static final int INVISIBLE = 0x00000004;

    /**
     * This view is invisible, and it doesn't take any space for layout
     * purposes. Use with {@link #setVisibility} and <a href="#attr_android:visibility">{@code
     * android:visibility}.
     */
    public static final int GONE = 0x00000008;

    /**
     * Mask for use with setFlags indicating bits used for visibility.
     * {@hide}
     */
    static final int VISIBILITY_MASK = 0x0000000C;

    private static final int[] VISIBILITY_FLAGS = {VISIBLE, INVISIBLE, GONE};

    /**
     * This view is enabled. Interpretation varies by subclass.
     * Use with ENABLED_MASK when calling setFlags.
     * {@hide}
     */
    static final int ENABLED = 0x00000000;

    /**
     * This view is disabled. Interpretation varies by subclass.
     * Use with ENABLED_MASK when calling setFlags.
     * {@hide}
     */
    static final int DISABLED = 0x00000020;

   /**
    * Mask for use with setFlags indicating bits used for indicating whether
    * this view is enabled
    * {@hide}
    */
    static final int ENABLED_MASK = 0x00000020;

    /**
     * This view won't draw. {@link #onDraw(android.graphics.Canvas)} won't be
     * called and further optimizations will be performed. It is okay to have
     * this flag set and a background. Use with DRAW_MASK when calling setFlags.
     * {@hide}
     */
    static final int WILL_NOT_DRAW = 0x00000080;

    /**
     * Mask for use with setFlags indicating bits used for indicating whether
     * this view is will draw
     * {@hide}
     */
    static final int DRAW_MASK = 0x00000080;

    /**
     * <p>This view doesn't show scrollbars.</p>
     * {@hide}
     */
    static final int SCROLLBARS_NONE = 0x00000000;

    /**
     * <p>This view shows horizontal scrollbars.</p>
     * {@hide}
     */
    static final int SCROLLBARS_HORIZONTAL = 0x00000100;

    /**
     * <p>This view shows vertical scrollbars.</p>
     * {@hide}
     */
    static final int SCROLLBARS_VERTICAL = 0x00000200;

    /**
     * <p>Mask for use with setFlags indicating bits used for indicating which
     * scrollbars are enabled.</p>
     * {@hide}
     */
    static final int SCROLLBARS_MASK = 0x00000300;

    /**
     * Indicates that the view should filter touches when its window is obscured.
     * Refer to the class comments for more information about this security feature.
     * {@hide}
     */
    static final int FILTER_TOUCHES_WHEN_OBSCURED = 0x00000400;

    /**
     * Set for framework elements that use FITS_SYSTEM_WINDOWS, to indicate
     * that they are optional and should be skipped if the window has
     * requested system UI flags that ignore those insets for layout.
     */
    static final int OPTIONAL_FITS_SYSTEM_WINDOWS = 0x00000800;

    /**
     * <p>This view doesn't show fading edges.</p>
     * {@hide}
     */
    static final int FADING_EDGE_NONE = 0x00000000;

    /**
     * <p>This view shows horizontal fading edges.</p>
     * {@hide}
     */
    static final int FADING_EDGE_HORIZONTAL = 0x00001000;

    /**
     * <p>This view shows vertical fading edges.</p>
     * {@hide}
     */
    static final int FADING_EDGE_VERTICAL = 0x00002000;

    /**
     * <p>Mask for use with setFlags indicating bits used for indicating which
     * fading edges are enabled.</p>
     * {@hide}
     */
    static final int FADING_EDGE_MASK = 0x00003000;

    /**
     * <p>Indicates this view can be clicked. When clickable, a View reacts
     * to clicks by notifying the OnClickListener.<p>
     * {@hide}
     */
    static final int CLICKABLE = 0x00004000;

    /**
     * <p>Indicates this view is caching its drawing into a bitmap.</p>
     * {@hide}
     */
    static final int DRAWING_CACHE_ENABLED = 0x00008000;

    /**
     * <p>Indicates that no icicle should be saved for this view.<p>
     * {@hide}
     */
    static final int SAVE_DISABLED = 0x000010000;

    /**
     * <p>Mask for use with setFlags indicating bits used for the saveEnabled
     * property.</p>
     * {@hide}
     */
    static final int SAVE_DISABLED_MASK = 0x000010000;

    /**
     * <p>Indicates that no drawing cache should ever be created for this view.<p>
     * {@hide}
     */
    static final int WILL_NOT_CACHE_DRAWING = 0x000020000;

    /**
     * <p>Indicates this view can take / keep focus when int touch mode.</p>
     * {@hide}
     */
    static final int FOCUSABLE_IN_TOUCH_MODE = 0x00040000;

    /**
     * <p>Enables low quality mode for the drawing cache.</p>
     */
    public static final int DRAWING_CACHE_QUALITY_LOW = 0x00080000;

    /**
     * <p>Enables high quality mode for the drawing cache.</p>
     */
    public static final int DRAWING_CACHE_QUALITY_HIGH = 0x00100000;

    /**
     * <p>Enables automatic quality mode for the drawing cache.</p>
     */
    public static final int DRAWING_CACHE_QUALITY_AUTO = 0x00000000;

    private static final int[] DRAWING_CACHE_QUALITY_FLAGS = {
            DRAWING_CACHE_QUALITY_AUTO, DRAWING_CACHE_QUALITY_LOW, DRAWING_CACHE_QUALITY_HIGH
    };

    /**
     * <p>Mask for use with setFlags indicating bits used for the cache
     * quality property.</p>
     * {@hide}
     */
    static final int DRAWING_CACHE_QUALITY_MASK = 0x00180000;

    /**
     * <p>
     * Indicates this view can be long clicked. When long clickable, a View
     * reacts to long clicks by notifying the OnLongClickListener or showing a
     * context menu.
     * </p>
     * {@hide}
     */
    static final int LONG_CLICKABLE = 0x00200000;

    /**
     * <p>Indicates that this view gets its drawable states from its direct parent
     * and ignores its original internal states.</p>
     *
     * @hide
     */
    static final int DUPLICATE_PARENT_STATE = 0x00400000;

    /**
     * The scrollbar style to display the scrollbars inside the content area,
     * without increasing the padding. The scrollbars will be overlaid with
     * translucency on the view's content.
     */
    public static final int SCROLLBARS_INSIDE_OVERLAY = 0;

    /**
     * The scrollbar style to display the scrollbars inside the padded area,
     * increasing the padding of the view. The scrollbars will not overlap the
     * content area of the view.
     */
    public static final int SCROLLBARS_INSIDE_INSET = 0x01000000;

    /**
     * The scrollbar style to display the scrollbars at the edge of the view,
     * without increasing the padding. The scrollbars will be overlaid with
     * translucency.
     */
    public static final int SCROLLBARS_OUTSIDE_OVERLAY = 0x02000000;

    /**
     * The scrollbar style to display the scrollbars at the edge of the view,
     * increasing the padding of the view. The scrollbars will only overlap the
     * background, if any.
     */
    public static final int SCROLLBARS_OUTSIDE_INSET = 0x03000000;

    /**
     * Mask to check if the scrollbar style is overlay or inset.
     * {@hide}
     */
    static final int SCROLLBARS_INSET_MASK = 0x01000000;

    /**
     * Mask to check if the scrollbar style is inside or outside.
     * {@hide}
     */
    static final int SCROLLBARS_OUTSIDE_MASK = 0x02000000;

    /**
     * Mask for scrollbar style.
     * {@hide}
     */
    static final int SCROLLBARS_STYLE_MASK = 0x03000000;

    /**
     * View flag indicating that the screen should remain on while the
     * window containing this view is visible to the user.  This effectively
     * takes care of automatically setting the WindowManager's
     * {@link WindowManager.LayoutParams#FLAG_KEEP_SCREEN_ON}.
     */
    public static final int KEEP_SCREEN_ON = 0x04000000;

    /**
     * View flag indicating whether this view should have sound effects enabled
     * for events such as clicking and touching.
     */
    public static final int SOUND_EFFECTS_ENABLED = 0x08000000;

    /**
     * View flag indicating whether this view should have haptic feedback
     * enabled for events such as long presses.
     */
    public static final int HAPTIC_FEEDBACK_ENABLED = 0x10000000;

    /**
     * <p>Indicates that the view hierarchy should stop saving state when
     * it reaches this view.  If state saving is initiated immediately at
     * the view, it will be allowed.
     * {@hide}
     */
    static final int PARENT_SAVE_DISABLED = 0x20000000;

    /**
     * <p>Mask for use with setFlags indicating bits used for PARENT_SAVE_DISABLED.</p>
     * {@hide}
     */
    static final int PARENT_SAVE_DISABLED_MASK = 0x20000000;

    /**
     * View flag indicating whether {@link #addFocusables(ArrayList, int, int)}
     * should add all focusable Views regardless if they are focusable in touch mode.
     */
    public static final int FOCUSABLES_ALL = 0x00000000;

    /**
     * View flag indicating whether {@link #addFocusables(ArrayList, int, int)}
     * should add only Views focusable in touch mode.
     */
    public static final int FOCUSABLES_TOUCH_MODE = 0x00000001;

    /**
     * Use with {@link #focusSearch(int)}. Move focus to the previous selectable
     * item.
     */
    public static final int FOCUS_BACKWARD = 0x00000001;

    /**
     * Use with {@link #focusSearch(int)}. Move focus to the next selectable
     * item.
     */
    public static final int FOCUS_FORWARD = 0x00000002;

    /**
     * Use with {@link #focusSearch(int)}. Move focus to the left.
     */
    public static final int FOCUS_LEFT = 0x00000011;

    /**
     * Use with {@link #focusSearch(int)}. Move focus up.
     */
    public static final int FOCUS_UP = 0x00000021;

    /**
     * Use with {@link #focusSearch(int)}. Move focus to the right.
     */
    public static final int FOCUS_RIGHT = 0x00000042;

    /**
     * Use with {@link #focusSearch(int)}. Move focus down.
     */
    public static final int FOCUS_DOWN = 0x00000082;

    /**
     * Bits of {@link #getMeasuredWidthAndState()} and
     * {@link #getMeasuredWidthAndState()} that provide the actual measured size.
     */
    public static final int MEASURED_SIZE_MASK = 0x00ffffff;

    /**
     * Bits of {@link #getMeasuredWidthAndState()} and
     * {@link #getMeasuredWidthAndState()} that provide the additional state bits.
     */
    public static final int MEASURED_STATE_MASK = 0xff000000;

    /**
     * Bit shift of {@link #MEASURED_STATE_MASK} to get to the height bits
     * for functions that combine both width and height into a single int,
     * such as {@link #getMeasuredState()} and the childState argument of
     * {@link #resolveSizeAndState(int, int, int)}.
     */
    public static final int MEASURED_HEIGHT_STATE_SHIFT = 16;

    /**
     * Bit of {@link #getMeasuredWidthAndState()} and
     * {@link #getMeasuredWidthAndState()} that indicates the measured size
     * is smaller that the space the view would like to have.
     */
    public static final int MEASURED_STATE_TOO_SMALL = 0x01000000;

// --- and some flags

    /**
     * Indicates that this view has reported that it can accept the current drag's content.
     * Cleared when the drag operation concludes.
     * @hide
     */
    static final int PFLAG2_DRAG_CAN_ACCEPT            = 0x00000001;

    /**
     * Indicates that this view is currently directly under the drag location in a
     * drag-and-drop operation involving content that it can accept.  Cleared when
     * the drag exits the view, or when the drag operation concludes.
     * @hide
     */
    static final int PFLAG2_DRAG_HOVERED               = 0x00000002;

    /**
     * Horizontal layout direction of this view is from Left to Right.
     * Use with {@link #setLayoutDirection}.
     */
    public static final int LAYOUT_DIRECTION_LTR = LayoutDirection.LTR;

    /**
     * Horizontal layout direction of this view is from Right to Left.
     * Use with {@link #setLayoutDirection}.
     */
    public static final int LAYOUT_DIRECTION_RTL = LayoutDirection.RTL;

    /**
     * Horizontal layout direction of this view is inherited from its parent.
     * Use with {@link #setLayoutDirection}.
     */
    public static final int LAYOUT_DIRECTION_INHERIT = LayoutDirection.INHERIT;

    /**
     * Horizontal layout direction of this view is from deduced from the default language
     * script for the locale. Use with {@link #setLayoutDirection}.
     */
    public static final int LAYOUT_DIRECTION_LOCALE = LayoutDirection.LOCALE;

    /**
     * Bit shift to get the horizontal layout direction. (bits after DRAG_HOVERED)
     * @hide
     */
    static final int PFLAG2_LAYOUT_DIRECTION_MASK_SHIFT = 2;

    /**
     * Mask for use with private flags indicating bits used for horizontal layout direction.
     * @hide
     */
    static final int PFLAG2_LAYOUT_DIRECTION_MASK = 0x00000003 << PFLAG2_LAYOUT_DIRECTION_MASK_SHIFT;

    /**
     * Indicates whether the view horizontal layout direction has been resolved and drawn to the
     * right-to-left direction.
     * @hide
     */
    static final int PFLAG2_LAYOUT_DIRECTION_RESOLVED_RTL = 4 << PFLAG2_LAYOUT_DIRECTION_MASK_SHIFT;

    /**
     * Indicates whether the view horizontal layout direction has been resolved.
     * @hide
     */
    static final int PFLAG2_LAYOUT_DIRECTION_RESOLVED = 8 << PFLAG2_LAYOUT_DIRECTION_MASK_SHIFT;

    /**
     * Mask for use with private flags indicating bits used for resolved horizontal layout direction.
     * @hide
     */
    static final int PFLAG2_LAYOUT_DIRECTION_RESOLVED_MASK = 0x0000000C << PFLAG2_LAYOUT_DIRECTION_MASK_SHIFT;

// --- end of constants from android source

// --- interfaces from android source
    public interface OnTouchListener {
        /**
         * Called when a touch event is dispatched to a view. This allows listeners to
         * get a chance to respond before the target view.
         *
         * @param v The view the touch event has been dispatched to.
         * @param event The MotionEvent object containing full information about
         *        the event.
         * @return True if the listener has consumed the event, false otherwise.
         */
        boolean onTouch(View v, MotionEvent event);
    }

    public interface OnClickListener {
        void onClick(View v);
    }

	public interface OnAttachStateChangeListener {
		// TODO
	}

	public static interface OnKeyListener {
// TODO
//		boolean onKey(View v, int keyCode, KeyEvent event);
	}

// --- end of interfaces

// --- subclasses

    /**
     * A MeasureSpec encapsulates the layout requirements passed from parent to child.
     * Each MeasureSpec represents a requirement for either the width or the height.
     * A MeasureSpec is comprised of a size and a mode. There are three possible
     * modes:
     * <dl>
     * <dt>UNSPECIFIED</dt>
     * <dd>
     * The parent has not imposed any constraint on the child. It can be whatever size
     * it wants.
     * </dd>
     *
     * <dt>EXACTLY</dt>
     * <dd>
     * The parent has determined an exact size for the child. The child is going to be
     * given those bounds regardless of how big it wants to be.
     * </dd>
     *
     * <dt>AT_MOST</dt>
     * <dd>
     * The child can be as large as it wants up to the specified size.
     * </dd>
     * </dl>
     *
     * MeasureSpecs are implemented as ints to reduce object allocation. This class
     * is provided to pack and unpack the &lt;size, mode&gt; tuple into the int.
     */
    public static class MeasureSpec {
        private static final int MODE_SHIFT = 30;
        private static final int MODE_MASK  = 0x3 << MODE_SHIFT;

        /**
         * Measure specification mode: The parent has not imposed any constraint
         * on the child. It can be whatever size it wants.
         */
        public static final int UNSPECIFIED = 0 << MODE_SHIFT;

        /**
         * Measure specification mode: The parent has determined an exact size
         * for the child. The child is going to be given those bounds regardless
         * of how big it wants to be.
         */
        public static final int EXACTLY     = 1 << MODE_SHIFT;

        /**
         * Measure specification mode: The child can be as large as it wants up
         * to the specified size.
         */
        public static final int AT_MOST     = 2 << MODE_SHIFT;

        /**
         * Creates a measure specification based on the supplied size and mode.
         *
         * The mode must always be one of the following:
         * <ul>
         *  <li>{@link android.view.View.MeasureSpec#UNSPECIFIED}</li>
         *  <li>{@link android.view.View.MeasureSpec#EXACTLY}</li>
         *  <li>{@link android.view.View.MeasureSpec#AT_MOST}</li>
         * </ul>
         *
         * <p><strong>Note:</strong> On API level 17 and lower, makeMeasureSpec's
         * implementation was such that the order of arguments did not matter
         * and overflow in either value could impact the resulting MeasureSpec.
         * {@link android.widget.RelativeLayout} was affected by this bug.
         * Apps targeting API levels greater than 17 will get the fixed, more strict
         * behavior.</p>
         *
         * @param size the size of the measure specification
         * @param mode the mode of the measure specification
         * @return the measure specification based on size and mode
         */
        public static int makeMeasureSpec(int size, int mode) {
            if (sUseBrokenMakeMeasureSpec) {
                return size + mode;
            } else {
                return (size & ~MODE_MASK) | (mode & MODE_MASK);
            }
        }

        /**
         * Extracts the mode from the supplied measure specification.
         *
         * @param measureSpec the measure specification to extract the mode from
         * @return {@link android.view.View.MeasureSpec#UNSPECIFIED},
         *         {@link android.view.View.MeasureSpec#AT_MOST} or
         *         {@link android.view.View.MeasureSpec#EXACTLY}
         */
        public static int getMode(int measureSpec) {
            return (measureSpec & MODE_MASK);
        }

        /**
         * Extracts the size from the supplied measure specification.
         *
         * @param measureSpec the measure specification to extract the size from
         * @return the size in pixels defined in the supplied measure specification
         */
        public static int getSize(int measureSpec) {
            return (measureSpec & ~MODE_MASK);
        }

        static int adjust(int measureSpec, int delta) {
            return makeMeasureSpec(getSize(measureSpec + delta), getMode(measureSpec));
        }

        /**
         * Returns a String representation of the specified measure
         * specification.
         *
         * @param measureSpec the measure specification to convert to a String
         * @return a String with the following format: "MeasureSpec: MODE SIZE"
         */
        public static String toString(int measureSpec) {
            int mode = getMode(measureSpec);
            int size = getSize(measureSpec);

            StringBuilder sb = new StringBuilder("MeasureSpec: ");

            if (mode == UNSPECIFIED)
                sb.append("UNSPECIFIED ");
            else if (mode == EXACTLY)
                sb.append("EXACTLY ");
            else if (mode == AT_MOST)
                sb.append("AT_MOST ");
            else
                sb.append(mode).append(" ");

            sb.append(size);
            return sb.toString();
        }
    }

// --- end of subclasses

	public int id;
	public ViewGroup parent;
	public AttributeSet attrs;
	protected ViewGroup.LayoutParams layout_params;
	private Context context;

	public long widget; // pointer

	public static HashMap<Integer, View> view_by_id = new HashMap<Integer, View>();

	public View() {} // FIXME

	public View(AttributeSet attrs) {
		id = attrs.getAttributeIntValue("http://schemas.android.com/apk/res/android", "id", 0);

		if(id != 0)
			view_by_id.put(id, this);
	}

	public View findViewById(int id) {
		/* TODO: this should probably only search among children, but the id is surely unique anyway? :P */
		return view_by_id.get(id);
	}

	public void onDraw(Canvas canvas) {}

	public View(Context context) {
		boolean custom_drawing;

		this.context = context;

		try {
			Class[] cArg = new Class[1];
			cArg[0] = Canvas.class;
			custom_drawing = !( this.getClass().getMethod("onDraw", cArg).getDeclaringClass() == View.class );
		} catch (NoSuchMethodException e) {
			custom_drawing = false;
		}

		if(custom_drawing) {
			native_constructor(context);
		}
	}

	public final ViewParent getParent() {
		return parent;
	}

	public void setLayoutParams(ViewGroup.LayoutParams params) {
		if (params == null) {
			throw new NullPointerException("Layout parameters cannot be null");
		}

		native_set_size_request(params.width, params.height);

		if(params.gravity != -1) {
			setGravity(params.gravity);
		}

		layout_params = params;
	}

	public ViewGroup.LayoutParams getLayoutParams() {
		return layout_params;
	}

	protected final void setMeasuredDimension(int measuredWidth, int measuredHeight) {
		native_set_size_request(MeasureSpec.getSize(measuredWidth), MeasureSpec.getSize(measuredHeight));
	}

	public native void setGravity(int gravity);
	public native void setOnTouchListener(OnTouchListener l);
	public native void setOnClickListener(OnClickListener l);
    public native final int getWidth();
    public native final int getHeight();

	private native void native_constructor(Context context); // will create a custom GtkWidget with a custom drawing function
	private native void native_set_size_request(int width, int height);

// --- stubs

	public void setOnKeyListener(OnKeyListener l) {}

	public void setFocusable(boolean focusable) {}
	public void setFocusableInTouchMode(boolean focusableInTouchMode) {}
    public final boolean requestFocus() {
        return requestFocus(View.FOCUS_DOWN);
    }
    public final boolean requestFocus(int direction) {
        return requestFocus(direction, null);
    }
    public boolean requestFocus(int direction, Rect previouslyFocusedRect) {
        return true;
    }

	public void setSystemUiVisibility(int visibility) {}

	protected void onMeasure(int widthMeasureSpec, int heightMeasureSpec) {}

	public void setPressed(boolean pressed) {
		System.out.println("calling setPressed on "+this+" with value: "+pressed);
	}

	public void setSelected(boolean selected) {
		System.out.println("calling setSelected on "+this+" with value: "+selected);
	}


	public ViewTreeObserver getViewTreeObserver() {
		return new ViewTreeObserver();
	}

	protected void onFinishInflate() {}

	public void invalidate(Rect dirty) {}
	public void invalidate(int l, int t, int r, int b) {}
	public void invalidate() {}

	public void setBackgroundColor(int color) {}
	public native void setVisibility(int visibility);
	public void setPadding(int left, int top, int right, int bottom) {}
	public void setBackgroundResource(int resid) {
//		System.out.println("*** setBackgroundResource: " + getString(resid));
	}

	public void getHitRect(Rect outRect) {}
	public final boolean getLocalVisibleRect(Rect r) { return false; }

	public final int getTop() { return 0; }
	public final int getScrollY() { return 0; }

	public void scrollTo(int x, int y) {}

	public void getLocationOnScreen(int[] location) { // FIXME: actually return the widget's location (and also have the onclick callback convert to window coordinates, because is seems that's what android deals in..)
		location[0] = 0;
		location[1] = 0;
	}

	public boolean performHapticFeedback(int feedbackConstant) {
		return performHapticFeedback(feedbackConstant, 0);
	}

	public boolean performHapticFeedback(int feedbackConstant, int flags) {
		System.out.println("vibration motor go burrrr");
		return true; // FIXME why is it not void
	}

	public int getPaddingLeft() {
		return 0;
	}

	public int getPaddingRight() {
		return 0;
	}

	public int getPaddingTop() {
		return 0;
	}

	public int getPaddingBottom() {
		return 0;
	}

	public void postInvalidate() {}

	public void postInvalidate(int left, int top, int right, int bottom) {
		System.out.println("postInvalidate("+left+","+top+","+right+","+bottom+") called");
	}
}