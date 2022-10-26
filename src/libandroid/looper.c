#include <stddef.h>

struct ALooper {
	int dummy;
};

struct ALooper a_looper;

struct ALooper* ALooper_forThread()
{
	return &a_looper;
}

struct ALooper* ALooper_prepare(int opts)
{
	return NULL;
}

int ALooper_pollAll(int timeoutMillis, int* outFd, int* outEvents, void** outData)
{
	return 0;
}
