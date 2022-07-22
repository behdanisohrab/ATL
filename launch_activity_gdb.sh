#!/bin/sh
# only architectures with 32bit integers and pointers can be supported
# without massive rewrite of dalvik, which is probably part of why ART exists
ARCH=x86
CURRDIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

# this might be needed on X11, but it only works with debug builds of Gtk
export GDK_DEBUG=gl-egl

# these are required by libdvm.so (dalvik vm); we also use ANDROID_ROOT to get absolute paths for bootclasspath
export ANDROID_ROOT=${CURRDIR}/dalvik/linux-${ARCH}
export ANDROID_DATA=/tmp/dalvik-data

# compat
export LD_PRELOAD=libpthread_bio.so:libbsd.so.0

# libraries under this path will be loaded using shim bionic linker
export BIONIC_LD_LIBRARY_PATH=data/lib/

# places where we have libs; dalvik libs, the application's libs, and our reimplementation effort libs respectively
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:${ANDROID_ROOT}/lib/:${CURRDIR}/data/lib/:${CURRDIR}/libnative/

gdb --args ${CURRDIR}/main ${1} -l ${2}
