#!/bin/sh
LD_PRELOAD=libpthread_bio.so:libbsd.so.0 ./dalvik/gdalvik -verbose:jni -cp hax_arsc_parser.dex:hax.dex:main.dex:${1}:com.google.android.gms.apk org/launch/main -l ${2}
