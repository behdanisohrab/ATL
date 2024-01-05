default: all

LOCAL_SRC_FILES := \
	ExecStrings.cpp \
	IcuUtilities.cpp \
	JniException.cpp \
	NetworkUtilities.cpp \
	Register.cpp \
	ZipUtilities.cpp \
	android_system_OsConstants.cpp \
	libcore_icu_ICU.cpp \
	libcore_icu_NativeConverter.cpp \
	libcore_icu_TimeZoneNames.cpp \
	libcore_io_AsynchronousCloseMonitor.cpp \
	libcore_io_Linux.cpp \
	libcore_io_Memory.cpp \
	libcore_util_NativeAllocationRegistry.cpp \
	org_apache_harmony_xml_ExpatParser.cpp \
	valueOf.cpp

all:
	gcc -shared -o libjavacore.so -fPIC -I./includes/nativehelper/ -I./includes -licuuc -licui18n -L/usr/local/lib64/art/ -llog -lutils -lbacktrace -lnativehelper -DHAVE_SYS_UIO_H ${LOCAL_SRC_FILES} -g
