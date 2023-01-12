#include <stddef.h>
#include <stdio.h>

typedef void ALooper;
typedef int (*Looper_callbackFunc)(int fd, int events, void* data);

/* helpers for creating wrappers for methods which use C++ itanium ABI */

#ifdef __aarch64__
	#define ITANIUM_OBJRETCALL_DEC(ret_type, mangled_name, ...) \
	void mangled_name(__VA_ARGS__); /* r8 used instead of implicit parameter, fun innit */
#else
	#define ITANIUM_OBJRETCALL_DEC(ret_type, mangled_name, ...) \
	void mangled_name(ret_type **ret, ##__VA_ARGS__) /* implicit first parameter used for return value */
#endif

inline __attribute__((__always_inline__)) void * itanium_objretcall(void (*func)(), ...)
{
#ifdef __aarch64__
	void *ret;
	register void **addr_of_ret asm("r8"); /* arm wants to be special :( */
	__asm__ ("" : : "" (addr_of_ret)); /* apparently __attribute__((used)) is not a thing */
	addr_of_ret = &ret;
	func(__builtin_va_arg_pack());
	return ret;
#else
	void *ret;
	func(&ret, __builtin_va_arg_pack());
	return ret;
#endif
}

/* --- */

ITANIUM_OBJRETCALL_DEC(ALooper, _ZN7android6Looper12getForThreadEv);
ALooper * ALooper_forThread(void)
{
	return itanium_objretcall(_ZN7android6Looper12getForThreadEv);
}

ITANIUM_OBJRETCALL_DEC(ALooper, _ZN7android6Looper7prepareEi, int opts);
ALooper * ALooper_prepare(int opts)
{
	return itanium_objretcall(_ZN7android6Looper7prepareEi, opts);
}

void _ZNK7android7RefBase9incStrongEPKv(ALooper *this, void *unused);
void ALooper_acquire(ALooper* looper) {
	_ZNK7android7RefBase9incStrongEPKv(looper, (void*)ALooper_acquire);
}

void _ZNK7android7RefBase9decStrongEPKv(ALooper *this, void *unused);
void ALooper_release(ALooper* looper) {
    _ZNK7android7RefBase9decStrongEPKv(looper, (void*)ALooper_acquire);
}

int _ZN7android6Looper7pollAllEiPiS1_PPv(ALooper *this, int timeoutMillis, int* outFd, int* outEvents, void** outData);
int ALooper_pollAll(int timeoutMillis, int* outFd, int* outEvents, void** outData)
{
	ALooper *looper = ALooper_forThread();
	if(!looper) {
		fprintf(stderr, "ALooper_pollAll: ALooper_forThread returned NULL\n");
		return 0;
	}

	return _ZN7android6Looper7pollAllEiPiS1_PPv(looper, timeoutMillis, outFd, outEvents,  outData);
}

int _ZN7android6Looper5addFdEiiiPFiiiPvES1_(ALooper *this, int fd, int ident, int events, Looper_callbackFunc callback, void* data);
int ALooper_addFd(ALooper* looper, int fd, int ident, int events, Looper_callbackFunc callback, void* data)
{
	return _ZN7android6Looper5addFdEiiiPFiiiPvES1_(looper, fd, ident, events, callback, data);
}
void _ZN7android6Looper4wakeEv(ALooper *this);
void ALooper_wake(ALooper* looper) {
    _ZN7android6Looper4wakeEv(looper);
}
