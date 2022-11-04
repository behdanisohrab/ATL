
typedef void JNIEnv;
typedef void AndroidBitmapInfo;
typedef void* jobject;

#define ANDROID_BITMAP_RESULT_SUCCESS 0


int AndroidBitmap_getInfo(JNIEnv* env, jobject jbitmap,
                          AndroidBitmapInfo* info) {
    return ANDROID_BITMAP_RESULT_SUCCESS;
}
int AndroidBitmap_lockPixels(JNIEnv* env, jobject jbitmap, void** addrPtr) {
    return ANDROID_BITMAP_RESULT_SUCCESS;
}
int AndroidBitmap_unlockPixels(JNIEnv* env, jobject jbitmap) {
    return ANDROID_BITMAP_RESULT_SUCCESS;
}
