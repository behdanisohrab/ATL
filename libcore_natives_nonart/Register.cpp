/*
 * Copyright (C) 2010 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#define LOG_TAG "libcore" // We'll be next to "dalvikvm" in the log; make the distinction clear.

#include <stdlib.h>

#include "log/log.h"

#include <nativehelper/JniConstants.h>
#include <nativehelper/ScopedLocalFrame.h>

// from JniConstants
static jclass findClass(JNIEnv* env, const char* name) {
    jclass result = reinterpret_cast<jclass>(env->NewGlobalRef(env->FindClass(name)));
    if (result == NULL) {
        ALOGE("failed to find class '%s'", name);
        abort();
    }
    return result;
}

// DalvikVM calls this on startup, so we can statically register all our native methods.
jint JNI_OnLoad(JavaVM* vm, void*) {
    JNIEnv* env;
    if (vm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6) != JNI_OK) {
        ALOGE("JavaVM::GetEnv() failed");
        abort();
    }

    // this would be done by something else normally
    JniConstants::booleanClass = findClass(env, "java/lang/Boolean");
    JniConstants::byteArrayClass = findClass(env, "[B");
    JniConstants::calendarClass = findClass(env, "java/util/Calendar");
    //JniConstants::charsetICUClass = findClass(env, "java/nio/charset/CharsetICU");
    JniConstants::doubleClass = findClass(env, "java/lang/Double");
    JniConstants::errnoExceptionClass = findClass(env, "android/system/ErrnoException");
    JniConstants::fileDescriptorClass = findClass(env, "java/io/FileDescriptor");
    JniConstants::gaiExceptionClass = findClass(env, "android/system/GaiException");
    JniConstants::inet6AddressClass = findClass(env, "java/net/Inet6Address");
    JniConstants::inet6AddressHolderClass = findClass(env, "java/net/Inet6Address$Inet6AddressHolder");
    JniConstants::inetAddressClass = findClass(env, "java/net/InetAddress");
    JniConstants::inetAddressHolderClass = findClass(env, "java/net/InetAddress$InetAddressHolder");
    JniConstants::inetSocketAddressClass = findClass(env, "java/net/InetSocketAddress");
    JniConstants::inetSocketAddressHolderClass = findClass(env, "java/net/InetSocketAddress$InetSocketAddressHolder");
    JniConstants::integerClass = findClass(env, "java/lang/Integer");
    //JniConstants::localeDataClass = findClass(env, "libcore/icu/LocaleData");
    JniConstants::longClass = findClass(env, "java/lang/Long");
    JniConstants::mutableIntClass = findClass(env, "android/util/MutableInt");
    JniConstants::mutableLongClass = findClass(env, "android/util/MutableLong");
    JniConstants::netlinkSocketAddressClass = findClass(env, "android/system/NetlinkSocketAddress");
    JniConstants::packetSocketAddressClass = findClass(env, "android/system/PacketSocketAddress");
    JniConstants::patternSyntaxExceptionClass = findClass(env, "java/util/regex/PatternSyntaxException");
    JniConstants::referenceClass = findClass(env, "java/lang/ref/Reference");
    JniConstants::socketTaggerClass = findClass(env, "dalvik/system/SocketTagger");
    JniConstants::stringClass = findClass(env, "java/lang/String");
    JniConstants::structAddrinfoClass = findClass(env, "android/system/StructAddrinfo");
    JniConstants::structFlockClass = findClass(env, "android/system/StructFlock");
    JniConstants::structGroupReqClass = findClass(env, "android/system/StructGroupReq");
    JniConstants::structGroupSourceReqClass = findClass(env, "android/system/StructGroupSourceReq");
    JniConstants::structIfaddrs = findClass(env, "android/system/StructIfaddrs");
    JniConstants::structLingerClass = findClass(env, "android/system/StructLinger");
    JniConstants::structPasswdClass = findClass(env, "android/system/StructPasswd");
    JniConstants::structPollfdClass = findClass(env, "android/system/StructPollfd");
    JniConstants::structStatClass = findClass(env, "android/system/StructStat");
    JniConstants::structStatVfsClass = findClass(env, "android/system/StructStatVfs");
    JniConstants::structTimevalClass = findClass(env, "android/system/StructTimeval");
    JniConstants::structTimespecClass = findClass(env, "android/system/StructTimespec");
    JniConstants::structUcredClass = findClass(env, "android/system/StructUcred");
    JniConstants::structUtsnameClass = findClass(env, "android/system/StructUtsname");
    //JniConstants::unixSocketAddressClass = findClass(env, "android/system/UnixSocketAddress");
    JniConstants::zipEntryClass = findClass(env, "java/util/zip/ZipEntry");
    // end of JniConstants init

    ScopedLocalFrame localFrame(env);

#define REGISTER(FN) extern void FN(JNIEnv*); FN(env)
    //REGISTER(register_android_system_OsConstants);
    REGISTER(register_libcore_icu_ICU);
//    REGISTER(register_libcore_icu_NativeConverter);
    REGISTER(register_libcore_icu_TimeZoneNames);
    REGISTER(register_libcore_io_AsynchronousCloseMonitor);
    REGISTER(register_libcore_io_Linux);
    REGISTER(register_libcore_io_Memory);
    REGISTER(register_libcore_util_NativeAllocationRegistry);
//    REGISTER(register_org_apache_harmony_dalvik_NativeTestTarget);
//    REGISTER(register_org_apache_harmony_xml_ExpatParser);
#undef REGISTER

    return JNI_VERSION_1_6;
}

// DalvikVM calls this on shutdown, do any global cleanup here.
// -- Very important if we restart multiple DalvikVMs in the same process to reset the state.
void JNI_OnUnload(JavaVM* vm, void*) {
    JNIEnv* env;
    if (vm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6) != JNI_OK) {
        ALOGE("JavaVM::GetEnv() failed");
        abort();
    }
    ALOGV("libjavacore JNI_OnUnload");

    ScopedLocalFrame localFrame(env);

#define UNREGISTER(FN) extern void FN(JNIEnv*); FN(env)
    UNREGISTER(unregister_libcore_icu_ICU);
#undef UNREGISTER
}
