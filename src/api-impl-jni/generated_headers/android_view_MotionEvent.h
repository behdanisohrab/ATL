/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class android_view_MotionEvent */

#ifndef _Included_android_view_MotionEvent
#define _Included_android_view_MotionEvent
#ifdef __cplusplus
extern "C" {
#endif
#undef android_view_MotionEvent_PARCEL_TOKEN_MOTION_EVENT
#define android_view_MotionEvent_PARCEL_TOKEN_MOTION_EVENT 1L
#undef android_view_MotionEvent_PARCEL_TOKEN_KEY_EVENT
#define android_view_MotionEvent_PARCEL_TOKEN_KEY_EVENT 2L
#undef android_view_MotionEvent_TRACK_RECYCLED_LOCATION
#define android_view_MotionEvent_TRACK_RECYCLED_LOCATION 0L
#undef android_view_MotionEvent_NS_PER_MS
#define android_view_MotionEvent_NS_PER_MS 1000000LL
#undef android_view_MotionEvent_INVALID_POINTER_ID
#define android_view_MotionEvent_INVALID_POINTER_ID -1L
#undef android_view_MotionEvent_ACTION_MASK
#define android_view_MotionEvent_ACTION_MASK 255L
#undef android_view_MotionEvent_ACTION_DOWN
#define android_view_MotionEvent_ACTION_DOWN 0L
#undef android_view_MotionEvent_ACTION_UP
#define android_view_MotionEvent_ACTION_UP 1L
#undef android_view_MotionEvent_ACTION_MOVE
#define android_view_MotionEvent_ACTION_MOVE 2L
#undef android_view_MotionEvent_ACTION_CANCEL
#define android_view_MotionEvent_ACTION_CANCEL 3L
#undef android_view_MotionEvent_ACTION_OUTSIDE
#define android_view_MotionEvent_ACTION_OUTSIDE 4L
#undef android_view_MotionEvent_ACTION_POINTER_DOWN
#define android_view_MotionEvent_ACTION_POINTER_DOWN 5L
#undef android_view_MotionEvent_ACTION_POINTER_UP
#define android_view_MotionEvent_ACTION_POINTER_UP 6L
#undef android_view_MotionEvent_ACTION_HOVER_MOVE
#define android_view_MotionEvent_ACTION_HOVER_MOVE 7L
#undef android_view_MotionEvent_ACTION_SCROLL
#define android_view_MotionEvent_ACTION_SCROLL 8L
#undef android_view_MotionEvent_ACTION_HOVER_ENTER
#define android_view_MotionEvent_ACTION_HOVER_ENTER 9L
#undef android_view_MotionEvent_ACTION_HOVER_EXIT
#define android_view_MotionEvent_ACTION_HOVER_EXIT 10L
#undef android_view_MotionEvent_ACTION_POINTER_INDEX_MASK
#define android_view_MotionEvent_ACTION_POINTER_INDEX_MASK 65280L
#undef android_view_MotionEvent_ACTION_POINTER_INDEX_SHIFT
#define android_view_MotionEvent_ACTION_POINTER_INDEX_SHIFT 8L
#undef android_view_MotionEvent_ACTION_POINTER_1_DOWN
#define android_view_MotionEvent_ACTION_POINTER_1_DOWN 5L
#undef android_view_MotionEvent_ACTION_POINTER_2_DOWN
#define android_view_MotionEvent_ACTION_POINTER_2_DOWN 261L
#undef android_view_MotionEvent_ACTION_POINTER_3_DOWN
#define android_view_MotionEvent_ACTION_POINTER_3_DOWN 517L
#undef android_view_MotionEvent_ACTION_POINTER_1_UP
#define android_view_MotionEvent_ACTION_POINTER_1_UP 6L
#undef android_view_MotionEvent_ACTION_POINTER_2_UP
#define android_view_MotionEvent_ACTION_POINTER_2_UP 262L
#undef android_view_MotionEvent_ACTION_POINTER_3_UP
#define android_view_MotionEvent_ACTION_POINTER_3_UP 518L
#undef android_view_MotionEvent_ACTION_POINTER_ID_MASK
#define android_view_MotionEvent_ACTION_POINTER_ID_MASK 65280L
#undef android_view_MotionEvent_ACTION_POINTER_ID_SHIFT
#define android_view_MotionEvent_ACTION_POINTER_ID_SHIFT 8L
#undef android_view_MotionEvent_FLAG_WINDOW_IS_OBSCURED
#define android_view_MotionEvent_FLAG_WINDOW_IS_OBSCURED 1L
#undef android_view_MotionEvent_FLAG_TAINTED
#define android_view_MotionEvent_FLAG_TAINTED -2147483648L
#undef android_view_MotionEvent_EDGE_TOP
#define android_view_MotionEvent_EDGE_TOP 1L
#undef android_view_MotionEvent_EDGE_BOTTOM
#define android_view_MotionEvent_EDGE_BOTTOM 2L
#undef android_view_MotionEvent_EDGE_LEFT
#define android_view_MotionEvent_EDGE_LEFT 4L
#undef android_view_MotionEvent_EDGE_RIGHT
#define android_view_MotionEvent_EDGE_RIGHT 8L
#undef android_view_MotionEvent_AXIS_X
#define android_view_MotionEvent_AXIS_X 0L
#undef android_view_MotionEvent_AXIS_Y
#define android_view_MotionEvent_AXIS_Y 1L
#undef android_view_MotionEvent_AXIS_PRESSURE
#define android_view_MotionEvent_AXIS_PRESSURE 2L
#undef android_view_MotionEvent_AXIS_SIZE
#define android_view_MotionEvent_AXIS_SIZE 3L
#undef android_view_MotionEvent_AXIS_TOUCH_MAJOR
#define android_view_MotionEvent_AXIS_TOUCH_MAJOR 4L
#undef android_view_MotionEvent_AXIS_TOUCH_MINOR
#define android_view_MotionEvent_AXIS_TOUCH_MINOR 5L
#undef android_view_MotionEvent_AXIS_TOOL_MAJOR
#define android_view_MotionEvent_AXIS_TOOL_MAJOR 6L
#undef android_view_MotionEvent_AXIS_TOOL_MINOR
#define android_view_MotionEvent_AXIS_TOOL_MINOR 7L
#undef android_view_MotionEvent_AXIS_ORIENTATION
#define android_view_MotionEvent_AXIS_ORIENTATION 8L
#undef android_view_MotionEvent_AXIS_VSCROLL
#define android_view_MotionEvent_AXIS_VSCROLL 9L
#undef android_view_MotionEvent_AXIS_HSCROLL
#define android_view_MotionEvent_AXIS_HSCROLL 10L
#undef android_view_MotionEvent_AXIS_Z
#define android_view_MotionEvent_AXIS_Z 11L
#undef android_view_MotionEvent_AXIS_RX
#define android_view_MotionEvent_AXIS_RX 12L
#undef android_view_MotionEvent_AXIS_RY
#define android_view_MotionEvent_AXIS_RY 13L
#undef android_view_MotionEvent_AXIS_RZ
#define android_view_MotionEvent_AXIS_RZ 14L
#undef android_view_MotionEvent_AXIS_HAT_X
#define android_view_MotionEvent_AXIS_HAT_X 15L
#undef android_view_MotionEvent_AXIS_HAT_Y
#define android_view_MotionEvent_AXIS_HAT_Y 16L
#undef android_view_MotionEvent_AXIS_LTRIGGER
#define android_view_MotionEvent_AXIS_LTRIGGER 17L
#undef android_view_MotionEvent_AXIS_RTRIGGER
#define android_view_MotionEvent_AXIS_RTRIGGER 18L
#undef android_view_MotionEvent_AXIS_THROTTLE
#define android_view_MotionEvent_AXIS_THROTTLE 19L
#undef android_view_MotionEvent_AXIS_RUDDER
#define android_view_MotionEvent_AXIS_RUDDER 20L
#undef android_view_MotionEvent_AXIS_WHEEL
#define android_view_MotionEvent_AXIS_WHEEL 21L
#undef android_view_MotionEvent_AXIS_GAS
#define android_view_MotionEvent_AXIS_GAS 22L
#undef android_view_MotionEvent_AXIS_BRAKE
#define android_view_MotionEvent_AXIS_BRAKE 23L
#undef android_view_MotionEvent_AXIS_DISTANCE
#define android_view_MotionEvent_AXIS_DISTANCE 24L
#undef android_view_MotionEvent_AXIS_TILT
#define android_view_MotionEvent_AXIS_TILT 25L
#undef android_view_MotionEvent_AXIS_GENERIC_1
#define android_view_MotionEvent_AXIS_GENERIC_1 32L
#undef android_view_MotionEvent_AXIS_GENERIC_2
#define android_view_MotionEvent_AXIS_GENERIC_2 33L
#undef android_view_MotionEvent_AXIS_GENERIC_3
#define android_view_MotionEvent_AXIS_GENERIC_3 34L
#undef android_view_MotionEvent_AXIS_GENERIC_4
#define android_view_MotionEvent_AXIS_GENERIC_4 35L
#undef android_view_MotionEvent_AXIS_GENERIC_5
#define android_view_MotionEvent_AXIS_GENERIC_5 36L
#undef android_view_MotionEvent_AXIS_GENERIC_6
#define android_view_MotionEvent_AXIS_GENERIC_6 37L
#undef android_view_MotionEvent_AXIS_GENERIC_7
#define android_view_MotionEvent_AXIS_GENERIC_7 38L
#undef android_view_MotionEvent_AXIS_GENERIC_8
#define android_view_MotionEvent_AXIS_GENERIC_8 39L
#undef android_view_MotionEvent_AXIS_GENERIC_9
#define android_view_MotionEvent_AXIS_GENERIC_9 40L
#undef android_view_MotionEvent_AXIS_GENERIC_10
#define android_view_MotionEvent_AXIS_GENERIC_10 41L
#undef android_view_MotionEvent_AXIS_GENERIC_11
#define android_view_MotionEvent_AXIS_GENERIC_11 42L
#undef android_view_MotionEvent_AXIS_GENERIC_12
#define android_view_MotionEvent_AXIS_GENERIC_12 43L
#undef android_view_MotionEvent_AXIS_GENERIC_13
#define android_view_MotionEvent_AXIS_GENERIC_13 44L
#undef android_view_MotionEvent_AXIS_GENERIC_14
#define android_view_MotionEvent_AXIS_GENERIC_14 45L
#undef android_view_MotionEvent_AXIS_GENERIC_15
#define android_view_MotionEvent_AXIS_GENERIC_15 46L
#undef android_view_MotionEvent_AXIS_GENERIC_16
#define android_view_MotionEvent_AXIS_GENERIC_16 47L
#undef android_view_MotionEvent_BUTTON_PRIMARY
#define android_view_MotionEvent_BUTTON_PRIMARY 1L
#undef android_view_MotionEvent_BUTTON_SECONDARY
#define android_view_MotionEvent_BUTTON_SECONDARY 2L
#undef android_view_MotionEvent_BUTTON_TERTIARY
#define android_view_MotionEvent_BUTTON_TERTIARY 4L
#undef android_view_MotionEvent_BUTTON_BACK
#define android_view_MotionEvent_BUTTON_BACK 8L
#undef android_view_MotionEvent_BUTTON_FORWARD
#define android_view_MotionEvent_BUTTON_FORWARD 16L
#undef android_view_MotionEvent_TOOL_TYPE_UNKNOWN
#define android_view_MotionEvent_TOOL_TYPE_UNKNOWN 0L
#undef android_view_MotionEvent_TOOL_TYPE_FINGER
#define android_view_MotionEvent_TOOL_TYPE_FINGER 1L
#undef android_view_MotionEvent_TOOL_TYPE_STYLUS
#define android_view_MotionEvent_TOOL_TYPE_STYLUS 2L
#undef android_view_MotionEvent_TOOL_TYPE_MOUSE
#define android_view_MotionEvent_TOOL_TYPE_MOUSE 3L
#undef android_view_MotionEvent_TOOL_TYPE_ERASER
#define android_view_MotionEvent_TOOL_TYPE_ERASER 4L
#undef android_view_MotionEvent_HISTORY_CURRENT
#define android_view_MotionEvent_HISTORY_CURRENT -2147483648L
#undef android_view_MotionEvent_MAX_RECYCLED
#define android_view_MotionEvent_MAX_RECYCLED 10L
/*
 * Class:     android_view_MotionEvent
 * Method:    nativeInitialize
 * Signature: (IIIIIIIIFFFFJJI[Landroid/view/MotionEvent/PointerProperties;[Landroid/view/MotionEvent/PointerCoords;)I
 */
JNIEXPORT jint JNICALL Java_android_view_MotionEvent_nativeInitialize
  (JNIEnv *, jclass, jint, jint, jint, jint, jint, jint, jint, jint, jfloat, jfloat, jfloat, jfloat, jlong, jlong, jint, jobjectArray, jobjectArray);

/*
 * Class:     android_view_MotionEvent
 * Method:    nativeCopy
 * Signature: (IIZ)I
 */
JNIEXPORT jint JNICALL Java_android_view_MotionEvent_nativeCopy
  (JNIEnv *, jclass, jint, jint, jboolean);

/*
 * Class:     android_view_MotionEvent
 * Method:    nativeDispose
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_android_view_MotionEvent_nativeDispose
  (JNIEnv *, jclass, jint);

/*
 * Class:     android_view_MotionEvent
 * Method:    nativeAddBatch
 * Signature: (IJ[Landroid/view/MotionEvent/PointerCoords;I)V
 */
JNIEXPORT void JNICALL Java_android_view_MotionEvent_nativeAddBatch
  (JNIEnv *, jclass, jint, jlong, jobjectArray, jint);

/*
 * Class:     android_view_MotionEvent
 * Method:    nativeGetDeviceId
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_android_view_MotionEvent_nativeGetDeviceId
  (JNIEnv *, jclass, jint);

/*
 * Class:     android_view_MotionEvent
 * Method:    nativeGetSource
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_android_view_MotionEvent_nativeGetSource
  (JNIEnv *, jclass, jint);

/*
 * Class:     android_view_MotionEvent
 * Method:    nativeSetSource
 * Signature: (II)I
 */
JNIEXPORT jint JNICALL Java_android_view_MotionEvent_nativeSetSource
  (JNIEnv *, jclass, jint, jint);

/*
 * Class:     android_view_MotionEvent
 * Method:    nativeGetAction
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_android_view_MotionEvent_nativeGetAction
  (JNIEnv *, jclass, jint);

/*
 * Class:     android_view_MotionEvent
 * Method:    nativeSetAction
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_android_view_MotionEvent_nativeSetAction
  (JNIEnv *, jclass, jint, jint);

/*
 * Class:     android_view_MotionEvent
 * Method:    nativeIsTouchEvent
 * Signature: (I)Z
 */
JNIEXPORT jboolean JNICALL Java_android_view_MotionEvent_nativeIsTouchEvent
  (JNIEnv *, jclass, jint);

/*
 * Class:     android_view_MotionEvent
 * Method:    nativeGetFlags
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_android_view_MotionEvent_nativeGetFlags
  (JNIEnv *, jclass, jint);

/*
 * Class:     android_view_MotionEvent
 * Method:    nativeSetFlags
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_android_view_MotionEvent_nativeSetFlags
  (JNIEnv *, jclass, jint, jint);

/*
 * Class:     android_view_MotionEvent
 * Method:    nativeGetEdgeFlags
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_android_view_MotionEvent_nativeGetEdgeFlags
  (JNIEnv *, jclass, jint);

/*
 * Class:     android_view_MotionEvent
 * Method:    nativeSetEdgeFlags
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_android_view_MotionEvent_nativeSetEdgeFlags
  (JNIEnv *, jclass, jint, jint);

/*
 * Class:     android_view_MotionEvent
 * Method:    nativeGetMetaState
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_android_view_MotionEvent_nativeGetMetaState
  (JNIEnv *, jclass, jint);

/*
 * Class:     android_view_MotionEvent
 * Method:    nativeGetButtonState
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_android_view_MotionEvent_nativeGetButtonState
  (JNIEnv *, jclass, jint);

/*
 * Class:     android_view_MotionEvent
 * Method:    nativeOffsetLocation
 * Signature: (IFF)V
 */
JNIEXPORT void JNICALL Java_android_view_MotionEvent_nativeOffsetLocation
  (JNIEnv *, jclass, jint, jfloat, jfloat);

/*
 * Class:     android_view_MotionEvent
 * Method:    nativeGetXOffset
 * Signature: (I)F
 */
JNIEXPORT jfloat JNICALL Java_android_view_MotionEvent_nativeGetXOffset
  (JNIEnv *, jclass, jint);

/*
 * Class:     android_view_MotionEvent
 * Method:    nativeGetYOffset
 * Signature: (I)F
 */
JNIEXPORT jfloat JNICALL Java_android_view_MotionEvent_nativeGetYOffset
  (JNIEnv *, jclass, jint);

/*
 * Class:     android_view_MotionEvent
 * Method:    nativeGetXPrecision
 * Signature: (I)F
 */
JNIEXPORT jfloat JNICALL Java_android_view_MotionEvent_nativeGetXPrecision
  (JNIEnv *, jclass, jint);

/*
 * Class:     android_view_MotionEvent
 * Method:    nativeGetYPrecision
 * Signature: (I)F
 */
JNIEXPORT jfloat JNICALL Java_android_view_MotionEvent_nativeGetYPrecision
  (JNIEnv *, jclass, jint);

/*
 * Class:     android_view_MotionEvent
 * Method:    nativeGetDownTimeNanos
 * Signature: (I)J
 */
JNIEXPORT jlong JNICALL Java_android_view_MotionEvent_nativeGetDownTimeNanos
  (JNIEnv *, jclass, jint);

/*
 * Class:     android_view_MotionEvent
 * Method:    nativeSetDownTimeNanos
 * Signature: (IJ)V
 */
JNIEXPORT void JNICALL Java_android_view_MotionEvent_nativeSetDownTimeNanos
  (JNIEnv *, jclass, jint, jlong);

/*
 * Class:     android_view_MotionEvent
 * Method:    nativeGetPointerCount
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_android_view_MotionEvent_nativeGetPointerCount
  (JNIEnv *, jclass, jint);

/*
 * Class:     android_view_MotionEvent
 * Method:    nativeGetPointerId
 * Signature: (II)I
 */
JNIEXPORT jint JNICALL Java_android_view_MotionEvent_nativeGetPointerId
  (JNIEnv *, jclass, jint, jint);

/*
 * Class:     android_view_MotionEvent
 * Method:    nativeGetToolType
 * Signature: (II)I
 */
JNIEXPORT jint JNICALL Java_android_view_MotionEvent_nativeGetToolType
  (JNIEnv *, jclass, jint, jint);

/*
 * Class:     android_view_MotionEvent
 * Method:    nativeFindPointerIndex
 * Signature: (II)I
 */
JNIEXPORT jint JNICALL Java_android_view_MotionEvent_nativeFindPointerIndex
  (JNIEnv *, jclass, jint, jint);

/*
 * Class:     android_view_MotionEvent
 * Method:    nativeGetHistorySize
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_android_view_MotionEvent_nativeGetHistorySize
  (JNIEnv *, jclass, jint);

/*
 * Class:     android_view_MotionEvent
 * Method:    nativeGetEventTimeNanos
 * Signature: (II)J
 */
JNIEXPORT jlong JNICALL Java_android_view_MotionEvent_nativeGetEventTimeNanos
  (JNIEnv *, jclass, jint, jint);

/*
 * Class:     android_view_MotionEvent
 * Method:    nativeGetRawAxisValue
 * Signature: (IIII)F
 */
JNIEXPORT jfloat JNICALL Java_android_view_MotionEvent_nativeGetRawAxisValue
  (JNIEnv *, jclass, jint, jint, jint, jint);

/*
 * Class:     android_view_MotionEvent
 * Method:    nativeGetPointerCoords
 * Signature: (IIILandroid/view/MotionEvent/PointerCoords;)V
 */
JNIEXPORT void JNICALL Java_android_view_MotionEvent_nativeGetPointerCoords
  (JNIEnv *, jclass, jint, jint, jint, jobject);

/*
 * Class:     android_view_MotionEvent
 * Method:    nativeGetPointerProperties
 * Signature: (IILandroid/view/MotionEvent/PointerProperties;)V
 */
JNIEXPORT void JNICALL Java_android_view_MotionEvent_nativeGetPointerProperties
  (JNIEnv *, jclass, jint, jint, jobject);

/*
 * Class:     android_view_MotionEvent
 * Method:    nativeScale
 * Signature: (IF)V
 */
JNIEXPORT void JNICALL Java_android_view_MotionEvent_nativeScale
  (JNIEnv *, jclass, jint, jfloat);

/*
 * Class:     android_view_MotionEvent
 * Method:    nativeTransform
 * Signature: (ILandroid/graphics/Matrix;)V
 */
JNIEXPORT void JNICALL Java_android_view_MotionEvent_nativeTransform
  (JNIEnv *, jclass, jint, jobject);

#ifdef __cplusplus
}
#endif
#endif
