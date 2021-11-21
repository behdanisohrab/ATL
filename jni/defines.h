#ifndef _DEFINES_H_
#define _DEFINES_H_

#define DEG2RAD(deg) (deg * M_PI / 180)

// these macros are a bit hacky, since they deliberately assume that env exists and refers to the JNI env

#define _PTR(ptr)((void*)(int)ptr)
#define _REF(obj)((*env)->NewGlobalRef(env, obj))
#define _CLASS(object) ((*env)->GetObjectClass(env, object))
#define _SUPER(object) ((*env)->GetSuperclass(env, object))
#define _METHOD(class, method, attrs) ((*env)->GetMethodID(env, class, method, attrs))
#define _JSTRING(cstring) ((*env)->NewStringUTF(env, cstring))
#define _CSTRING(jstring) ((*env)->GetStringUTFChars(env, jstring, NULL))
#define _FIELD_ID(class, field, type) ((*env)->GetFieldID(env, class , field, type))
#define _SET_OBJ_FIELD(object, field, type, value) ((*env)->SetObjectField(env, object, _FIELD_ID(_CLASS(object), field, type), value))
#define _GET_OBJ_FIELD(object, field, type) ((*env)->GetObjectField(env, object, _FIELD_ID(_CLASS(object), field, type)))
#define _SET_LONG_FIELD(object, field, value) ((*env)->SetLongField(env, object, _FIELD_ID(_CLASS(object), field, "J"), value))
#define _GET_LONG_FIELD(object, field) ((*env)->GetLongField(env, object, _FIELD_ID(_CLASS(object), field, "J")))
#define _SET_INT_FIELD(object, field, value) ((*env)->SetIntField(env, object, _FIELD_ID(_CLASS(object), field, "I"), value))
#define _GET_INT_FIELD(object, field) ((*env)->GetIntField(env, object, _FIELD_ID(_CLASS(object), field, "I")))
#define _GET_BYTE_ARRAY_ELEMENTS(b_array) ((*env)->GetByteArrayElements(env, b_array, NULL))
#define _RELEASE_BYTE_ARRAY_ELEMENTS(b_array, buffer_ptr) ((*env)->ReleaseByteArrayElements(env, b_array, buffer_ptr, 0))

#endif
