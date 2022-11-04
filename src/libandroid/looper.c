#include <stddef.h>
#include <stdio.h>

typedef void ALooper;
typedef int (*Looper_callbackFunc)(int fd, int events, void* data);

void _ZN7android6Looper12getForThreadEv(void **ret); // no clue why itanium ABI does this with return values
ALooper * ALooper_forThread()
{
	void *ret;
	_ZN7android6Looper12getForThreadEv(&ret);
	return ret;
}

void _ZN7android6Looper7prepareEi(void **ret, int opts); // no clue why itanium ABI does this with return values
ALooper * ALooper_prepare(int opts)
{
	void *ret;
	_ZN7android6Looper7prepareEi(&ret, opts);
	return ret;
}

int _ZN7android6Looper7pollAllEiPiS1_PPv(void *this, int timeoutMillis, int* outFd, int* outEvents, void** outData);
int ALooper_pollAll(int timeoutMillis, int* outFd, int* outEvents, void** outData)
{
	ALooper *looper = ALooper_forThread();
	if(!looper) {
		fprintf(stderr, "ALooper_pollAll: ALooper_forThread returned NULL\n");
		return 0;
	}

	return _ZN7android6Looper7pollAllEiPiS1_PPv(looper, timeoutMillis, outFd, outEvents,  outData);
}

int _ZN7android6Looper5addFdEiiiPFiiiPvES1_(void *this, int fd, int ident, int events, Looper_callbackFunc callback, void* data);
int ALooper_addFd(ALooper* looper, int fd, int ident, int events, Looper_callbackFunc callback, void* data)
{
	return _ZN7android6Looper5addFdEiiiPFiiiPvES1_(looper, fd, ident, events, callback, data);
}
