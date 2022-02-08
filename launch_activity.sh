#!/bin/sh
LD_PRELOAD=libpthread_bio.so ./dalvik/dalvik -verbose:jni -cp hax_arsc_parser.dex:hax_xmlpull.dex:hax.dex:main.dex:${1}:com.google.android.gms.apk org/launch/main ${2}
