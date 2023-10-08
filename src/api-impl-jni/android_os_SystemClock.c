#include <time.h>
#include <math.h>

#include "generated_headers/android_os_SystemClock.h"

JNIEXPORT jlong JNICALL Java_android_os_SystemClock_elapsedRealtime(JNIEnv *env, jclass this)
{
	struct timespec t;
	clock_gettime(CLOCK_BOOTTIME, &t);
	jlong ret = t.tv_sec * 1000 + lround(t.tv_nsec / 1e6);
    return ret;
}

JNIEXPORT jlong JNICALL Java_android_os_SystemClock_uptimeMillis(JNIEnv *env, jclass this)
{
    struct timespec t;
    clock_gettime(CLOCK_REALTIME, &t);
    return t.tv_sec * 1000 + lround(t.tv_nsec / 1e6);
}
