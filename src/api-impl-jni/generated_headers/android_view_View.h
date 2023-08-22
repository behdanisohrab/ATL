/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class android_view_View */

#ifndef _Included_android_view_View
#define _Included_android_view_View
#ifdef __cplusplus
extern "C" {
#endif
#undef android_view_View_NO_ID
#define android_view_View_NO_ID -1L
#undef android_view_View_NOT_FOCUSABLE
#define android_view_View_NOT_FOCUSABLE 0L
#undef android_view_View_FOCUSABLE
#define android_view_View_FOCUSABLE 1L
#undef android_view_View_FOCUSABLE_MASK
#define android_view_View_FOCUSABLE_MASK 1L
#undef android_view_View_FITS_SYSTEM_WINDOWS
#define android_view_View_FITS_SYSTEM_WINDOWS 2L
#undef android_view_View_VISIBLE
#define android_view_View_VISIBLE 0L
#undef android_view_View_INVISIBLE
#define android_view_View_INVISIBLE 4L
#undef android_view_View_GONE
#define android_view_View_GONE 8L
#undef android_view_View_VISIBILITY_MASK
#define android_view_View_VISIBILITY_MASK 12L
#undef android_view_View_ENABLED
#define android_view_View_ENABLED 0L
#undef android_view_View_DISABLED
#define android_view_View_DISABLED 32L
#undef android_view_View_ENABLED_MASK
#define android_view_View_ENABLED_MASK 32L
#undef android_view_View_WILL_NOT_DRAW
#define android_view_View_WILL_NOT_DRAW 128L
#undef android_view_View_DRAW_MASK
#define android_view_View_DRAW_MASK 128L
#undef android_view_View_SCROLLBARS_NONE
#define android_view_View_SCROLLBARS_NONE 0L
#undef android_view_View_SCROLLBARS_HORIZONTAL
#define android_view_View_SCROLLBARS_HORIZONTAL 256L
#undef android_view_View_SCROLLBARS_VERTICAL
#define android_view_View_SCROLLBARS_VERTICAL 512L
#undef android_view_View_SCROLLBARS_MASK
#define android_view_View_SCROLLBARS_MASK 768L
#undef android_view_View_FILTER_TOUCHES_WHEN_OBSCURED
#define android_view_View_FILTER_TOUCHES_WHEN_OBSCURED 1024L
#undef android_view_View_OPTIONAL_FITS_SYSTEM_WINDOWS
#define android_view_View_OPTIONAL_FITS_SYSTEM_WINDOWS 2048L
#undef android_view_View_FADING_EDGE_NONE
#define android_view_View_FADING_EDGE_NONE 0L
#undef android_view_View_FADING_EDGE_HORIZONTAL
#define android_view_View_FADING_EDGE_HORIZONTAL 4096L
#undef android_view_View_FADING_EDGE_VERTICAL
#define android_view_View_FADING_EDGE_VERTICAL 8192L
#undef android_view_View_FADING_EDGE_MASK
#define android_view_View_FADING_EDGE_MASK 12288L
#undef android_view_View_CLICKABLE
#define android_view_View_CLICKABLE 16384L
#undef android_view_View_DRAWING_CACHE_ENABLED
#define android_view_View_DRAWING_CACHE_ENABLED 32768L
#undef android_view_View_SAVE_DISABLED
#define android_view_View_SAVE_DISABLED 65536L
#undef android_view_View_SAVE_DISABLED_MASK
#define android_view_View_SAVE_DISABLED_MASK 65536L
#undef android_view_View_WILL_NOT_CACHE_DRAWING
#define android_view_View_WILL_NOT_CACHE_DRAWING 131072L
#undef android_view_View_FOCUSABLE_IN_TOUCH_MODE
#define android_view_View_FOCUSABLE_IN_TOUCH_MODE 262144L
#undef android_view_View_DRAWING_CACHE_QUALITY_LOW
#define android_view_View_DRAWING_CACHE_QUALITY_LOW 524288L
#undef android_view_View_DRAWING_CACHE_QUALITY_HIGH
#define android_view_View_DRAWING_CACHE_QUALITY_HIGH 1048576L
#undef android_view_View_DRAWING_CACHE_QUALITY_AUTO
#define android_view_View_DRAWING_CACHE_QUALITY_AUTO 0L
#undef android_view_View_DRAWING_CACHE_QUALITY_MASK
#define android_view_View_DRAWING_CACHE_QUALITY_MASK 1572864L
#undef android_view_View_LONG_CLICKABLE
#define android_view_View_LONG_CLICKABLE 2097152L
#undef android_view_View_DUPLICATE_PARENT_STATE
#define android_view_View_DUPLICATE_PARENT_STATE 4194304L
#undef android_view_View_SCROLLBARS_INSIDE_OVERLAY
#define android_view_View_SCROLLBARS_INSIDE_OVERLAY 0L
#undef android_view_View_SCROLLBARS_INSIDE_INSET
#define android_view_View_SCROLLBARS_INSIDE_INSET 16777216L
#undef android_view_View_SCROLLBARS_OUTSIDE_OVERLAY
#define android_view_View_SCROLLBARS_OUTSIDE_OVERLAY 33554432L
#undef android_view_View_SCROLLBARS_OUTSIDE_INSET
#define android_view_View_SCROLLBARS_OUTSIDE_INSET 50331648L
#undef android_view_View_SCROLLBARS_INSET_MASK
#define android_view_View_SCROLLBARS_INSET_MASK 16777216L
#undef android_view_View_SCROLLBARS_OUTSIDE_MASK
#define android_view_View_SCROLLBARS_OUTSIDE_MASK 33554432L
#undef android_view_View_SCROLLBARS_STYLE_MASK
#define android_view_View_SCROLLBARS_STYLE_MASK 50331648L
#undef android_view_View_KEEP_SCREEN_ON
#define android_view_View_KEEP_SCREEN_ON 67108864L
#undef android_view_View_SOUND_EFFECTS_ENABLED
#define android_view_View_SOUND_EFFECTS_ENABLED 134217728L
#undef android_view_View_HAPTIC_FEEDBACK_ENABLED
#define android_view_View_HAPTIC_FEEDBACK_ENABLED 268435456L
#undef android_view_View_PARENT_SAVE_DISABLED
#define android_view_View_PARENT_SAVE_DISABLED 536870912L
#undef android_view_View_PARENT_SAVE_DISABLED_MASK
#define android_view_View_PARENT_SAVE_DISABLED_MASK 536870912L
#undef android_view_View_FOCUSABLES_ALL
#define android_view_View_FOCUSABLES_ALL 0L
#undef android_view_View_FOCUSABLES_TOUCH_MODE
#define android_view_View_FOCUSABLES_TOUCH_MODE 1L
#undef android_view_View_FOCUS_BACKWARD
#define android_view_View_FOCUS_BACKWARD 1L
#undef android_view_View_FOCUS_FORWARD
#define android_view_View_FOCUS_FORWARD 2L
#undef android_view_View_FOCUS_LEFT
#define android_view_View_FOCUS_LEFT 17L
#undef android_view_View_FOCUS_UP
#define android_view_View_FOCUS_UP 33L
#undef android_view_View_FOCUS_RIGHT
#define android_view_View_FOCUS_RIGHT 66L
#undef android_view_View_FOCUS_DOWN
#define android_view_View_FOCUS_DOWN 130L
#undef android_view_View_MEASURED_SIZE_MASK
#define android_view_View_MEASURED_SIZE_MASK 16777215L
#undef android_view_View_MEASURED_STATE_MASK
#define android_view_View_MEASURED_STATE_MASK -16777216L
#undef android_view_View_MEASURED_HEIGHT_STATE_SHIFT
#define android_view_View_MEASURED_HEIGHT_STATE_SHIFT 16L
#undef android_view_View_MEASURED_STATE_TOO_SMALL
#define android_view_View_MEASURED_STATE_TOO_SMALL 16777216L
#undef android_view_View_PFLAG2_DRAG_CAN_ACCEPT
#define android_view_View_PFLAG2_DRAG_CAN_ACCEPT 1L
#undef android_view_View_PFLAG2_DRAG_HOVERED
#define android_view_View_PFLAG2_DRAG_HOVERED 2L
#undef android_view_View_LAYOUT_DIRECTION_LTR
#define android_view_View_LAYOUT_DIRECTION_LTR 0L
#undef android_view_View_LAYOUT_DIRECTION_RTL
#define android_view_View_LAYOUT_DIRECTION_RTL 1L
#undef android_view_View_LAYOUT_DIRECTION_INHERIT
#define android_view_View_LAYOUT_DIRECTION_INHERIT 2L
#undef android_view_View_LAYOUT_DIRECTION_LOCALE
#define android_view_View_LAYOUT_DIRECTION_LOCALE 3L
#undef android_view_View_PFLAG2_LAYOUT_DIRECTION_MASK_SHIFT
#define android_view_View_PFLAG2_LAYOUT_DIRECTION_MASK_SHIFT 2L
#undef android_view_View_PFLAG2_LAYOUT_DIRECTION_MASK
#define android_view_View_PFLAG2_LAYOUT_DIRECTION_MASK 12L
#undef android_view_View_PFLAG2_LAYOUT_DIRECTION_RESOLVED_RTL
#define android_view_View_PFLAG2_LAYOUT_DIRECTION_RESOLVED_RTL 16L
#undef android_view_View_PFLAG2_LAYOUT_DIRECTION_RESOLVED
#define android_view_View_PFLAG2_LAYOUT_DIRECTION_RESOLVED 32L
#undef android_view_View_PFLAG2_LAYOUT_DIRECTION_RESOLVED_MASK
#define android_view_View_PFLAG2_LAYOUT_DIRECTION_RESOLVED_MASK 48L
#undef android_view_View_STATUS_BAR_HIDDEN
#define android_view_View_STATUS_BAR_HIDDEN 1L
#undef android_view_View_STATUS_BAR_VISIBLE
#define android_view_View_STATUS_BAR_VISIBLE 0L
#undef android_view_View_SYSTEM_UI_FLAG_FULLSCREEN
#define android_view_View_SYSTEM_UI_FLAG_FULLSCREEN 4L
#undef android_view_View_SYSTEM_UI_FLAG_HIDE_NAVIGATION
#define android_view_View_SYSTEM_UI_FLAG_HIDE_NAVIGATION 2L
#undef android_view_View_SYSTEM_UI_FLAG_IMMERSIVE
#define android_view_View_SYSTEM_UI_FLAG_IMMERSIVE 2048L
#undef android_view_View_SYSTEM_UI_FLAG_IMMERSIVE_STICKY
#define android_view_View_SYSTEM_UI_FLAG_IMMERSIVE_STICKY 4096L
#undef android_view_View_SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN
#define android_view_View_SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN 1024L
#undef android_view_View_SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION
#define android_view_View_SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION 512L
#undef android_view_View_SYSTEM_UI_FLAG_LAYOUT_STABLE
#define android_view_View_SYSTEM_UI_FLAG_LAYOUT_STABLE 256L
#undef android_view_View_SYSTEM_UI_FLAG_LOW_PROFILE
#define android_view_View_SYSTEM_UI_FLAG_LOW_PROFILE 1L
#undef android_view_View_SYSTEM_UI_FLAG_VISIBLE
#define android_view_View_SYSTEM_UI_FLAG_VISIBLE 0L
#undef android_view_View_SYSTEM_UI_LAYOUT_FLAGS
#define android_view_View_SYSTEM_UI_LAYOUT_FLAGS 1536L
#undef android_view_View_TEXT_ALIGNMENT_CENTER
#define android_view_View_TEXT_ALIGNMENT_CENTER 4L
#undef android_view_View_TEXT_ALIGNMENT_GRAVITY
#define android_view_View_TEXT_ALIGNMENT_GRAVITY 1L
#undef android_view_View_TEXT_ALIGNMENT_INHERIT
#define android_view_View_TEXT_ALIGNMENT_INHERIT 0L
#undef android_view_View_TEXT_ALIGNMENT_TEXT_END
#define android_view_View_TEXT_ALIGNMENT_TEXT_END 3L
#undef android_view_View_TEXT_ALIGNMENT_TEXT_START
#define android_view_View_TEXT_ALIGNMENT_TEXT_START 2L
#undef android_view_View_TEXT_ALIGNMENT_VIEW_END
#define android_view_View_TEXT_ALIGNMENT_VIEW_END 6L
#undef android_view_View_TEXT_ALIGNMENT_VIEW_START
#define android_view_View_TEXT_ALIGNMENT_VIEW_START 5L
#undef android_view_View_TEXT_DIRECTION_ANY_RTL
#define android_view_View_TEXT_DIRECTION_ANY_RTL 2L
#undef android_view_View_TEXT_DIRECTION_FIRST_STRONG
#define android_view_View_TEXT_DIRECTION_FIRST_STRONG 1L
#undef android_view_View_TEXT_DIRECTION_INHERIT
#define android_view_View_TEXT_DIRECTION_INHERIT 0L
#undef android_view_View_TEXT_DIRECTION_LOCALE
#define android_view_View_TEXT_DIRECTION_LOCALE 5L
#undef android_view_View_TEXT_DIRECTION_LTR
#define android_view_View_TEXT_DIRECTION_LTR 3L
#undef android_view_View_TEXT_DIRECTION_RTL
#define android_view_View_TEXT_DIRECTION_RTL 4L
/*
 * Class:     android_view_View
 * Method:    setGravity
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_android_view_View_setGravity
  (JNIEnv *, jobject, jint);

/*
 * Class:     android_view_View
 * Method:    setOnTouchListener
 * Signature: (Landroid/view/View/OnTouchListener;)V
 */
JNIEXPORT void JNICALL Java_android_view_View_setOnTouchListener
  (JNIEnv *, jobject, jobject);

/*
 * Class:     android_view_View
 * Method:    setOnClickListener
 * Signature: (Landroid/view/View/OnClickListener;)V
 */
JNIEXPORT void JNICALL Java_android_view_View_setOnClickListener
  (JNIEnv *, jobject, jobject);

/*
 * Class:     android_view_View
 * Method:    getWidth
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_android_view_View_getWidth
  (JNIEnv *, jobject);

/*
 * Class:     android_view_View
 * Method:    getHeight
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_android_view_View_getHeight
  (JNIEnv *, jobject);

/*
 * Class:     android_view_View
 * Method:    native_constructor
 * Signature: (Landroid/content/Context;Landroid/util/AttributeSet;)J
 */
JNIEXPORT jlong JNICALL Java_android_view_View_native_1constructor
  (JNIEnv *, jobject, jobject, jobject);

/*
 * Class:     android_view_View
 * Method:    native_set_size_request
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_android_view_View_native_1set_1size_1request
  (JNIEnv *, jobject, jint, jint);

/*
 * Class:     android_view_View
 * Method:    native_destructor
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_android_view_View_native_1destructor
  (JNIEnv *, jobject, jlong);

/*
 * Class:     android_view_View
 * Method:    native_measure
 * Signature: (JII)V
 */
JNIEXPORT void JNICALL Java_android_view_View_native_1measure
  (JNIEnv *, jobject, jlong, jint, jint);

/*
 * Class:     android_view_View
 * Method:    native_layout
 * Signature: (JIIII)V
 */
JNIEXPORT void JNICALL Java_android_view_View_native_1layout
  (JNIEnv *, jobject, jlong, jint, jint, jint, jint);

/*
 * Class:     android_view_View
 * Method:    native_requestLayout
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_android_view_View_native_1requestLayout
  (JNIEnv *, jobject, jlong);

/*
 * Class:     android_view_View
 * Method:    nativeInvalidate
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_android_view_View_nativeInvalidate
  (JNIEnv *, jclass, jlong);

/*
 * Class:     android_view_View
 * Method:    setVisibility
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_android_view_View_setVisibility
  (JNIEnv *, jobject, jint);

#ifdef __cplusplus
}
#endif
#endif
