#ifndef LOOPER_H
#define LOOPER_H

typedef void ALooper;
ALooper * ALooper_prepare(int opts);
void ALooper_wake(ALooper *looper);
bool ALooper_isPolling(ALooper *looper);
int ALooper_pollOnce(int timeoutMillis, int* outFd, int* outEvents, void** outData);

#endif
