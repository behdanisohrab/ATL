#include <stddef.h>
#include <stdio.h>

typedef void ALooper;
typedef int (*Looper_callbackFunc)(int fd, int events, void* data);

/* macros for creating wrappers for methods which use C++ itanium ABI */

#ifdef __aarch64__

#define ITANIUM_OBJ_RET_WRAPPER_DEC(ret_type, wrapper_name, mangled_name, ...) \
void mangled_name(__VA_ARGS__); /* r8 used instead of implicit parameter, fun innit */\
ret_type * wrapper_name(__VA_ARGS__)

#define ITANIUM_OBJ_RET_WRAPPER_BODY(ret_type, wrapper_name, mangled_name, ...) \
{ \
	ret_type *ret; \
	register ret_type **addr_of_ret asm("r8"); /* arm wants to be special :( */\
	__asm__ ("" : : "" (addr_of_ret)); /* apparently __attribute__((used)) is not a thing */\
	addr_of_ret = &ret; \
	mangled_name(__VA_ARGS__); \
	return ret; \
}

#else

#define ITANIUM_OBJ_RET_WRAPPER_DEC(ret_type, wrapper_name, mangled_name, ...) \
void mangled_name(ret_type **ret, ##__VA_ARGS__); /* implicit first parameter used for return value */\
ret_type * wrapper_name(__VA_ARGS__)

#define ITANIUM_OBJ_RET_WRAPPER_BODY(ret_type, wrapper_name, mangled_name, ...) \
{ \
	ret_type *ret; \
	mangled_name(&ret, ##__VA_ARGS__); \
	return ret; \
}

#endif

#define ITANIUM_OBJ_RET_WRAPPER_NOARGS(ret_type, wrapper_name, mangled_name) \
ITANIUM_OBJ_RET_WRAPPER_DEC(ret_type, wrapper_name, mangled_name) \
ITANIUM_OBJ_RET_WRAPPER_BODY(ret_type, wrapper_name, mangled_name)

// ALooper * ALooper_forThread()
ITANIUM_OBJ_RET_WRAPPER_NOARGS(ALooper, ALooper_forThread, _ZN7android6Looper12getForThreadEv)

// ALooper * ALooper_prepare(int opts)
ITANIUM_OBJ_RET_WRAPPER_DEC(ALooper, ALooper_prepare, _ZN7android6Looper7prepareEi, int opts)
ITANIUM_OBJ_RET_WRAPPER_BODY(ALooper, ALooper_prepare, _ZN7android6Looper7prepareEi, opts)

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
