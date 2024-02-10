#include <GLES2/gl2.h>
#include <jni.h>
#include <stdint.h>
#include "defines.h"
#include "util.h"

#include "generated_headers/android_opengl_GLES20.h"

JNIEXPORT jstring JNICALL Java_android_opengl_GLES20_glGetString(JNIEnv *env, jclass, jint name) {
    const char* chars = (const char*) glGetString((GLenum) name);
    return _JSTRING(chars);
}

JNIEXPORT void JNICALL Java_android_opengl_GLES20_glGetIntegerv__I_3II(JNIEnv *env, jclass, jint pname, jintArray params_ref, jint offset) {
    jint *params = (*env)->GetIntArrayElements(env, params_ref, NULL);
    glGetIntegerv((GLenum)pname, params + offset);
    (*env)->ReleaseIntArrayElements(env, params_ref, params, 0);
}

JNIEXPORT void JNICALL Java_android_opengl_GLES20_glEnableVertexAttribArray(JNIEnv *env, jclass, jint index) {
    glEnableVertexAttribArray((GLuint)index);
}

JNIEXPORT void JNICALL Java_android_opengl_GLES20_glDisable(JNIEnv *env, jclass, jint cap) {
    glDisable((GLenum)cap);
}

JNIEXPORT void JNICALL Java_android_opengl_GLES20_glEnable(JNIEnv *env, jclass, jint cap) {
    glEnable((GLenum)cap);
}

JNIEXPORT void JNICALL Java_android_opengl_GLES20_glViewport(JNIEnv *env, jclass, jint x, jint y, jint width, jint height) {
    glViewport((GLint)x, (GLint)y, (GLsizei)width, (GLsizei)height);
}

JNIEXPORT void JNICALL Java_android_opengl_GLES20_glGenTextures__I_3II(JNIEnv *env, jclass, jint n, jintArray textures_ref, jint offset) {
    jint *textures = (*env)->GetIntArrayElements(env, textures_ref, NULL);
    glGenTextures((GLsizei)n, (GLuint *)textures + offset);
    (*env)->ReleaseIntArrayElements(env, textures_ref, textures, 0);
}

JNIEXPORT void JNICALL Java_android_opengl_GLES20_glBindTexture(JNIEnv *env, jclass, jint target, jint texture) {
    glBindTexture((GLenum)target, (GLuint)texture);
}

JNIEXPORT void JNICALL Java_android_opengl_GLES20_glTexImage2D(JNIEnv *env, jclass, jint target, jint level, jint internalformat, jint width, jint height, jint border, jint format, jint type, jobject pixels_buf) {
    jarray array_ref;
    jbyte *array;
    GLvoid *pixels = get_nio_buffer(env, pixels_buf, &array_ref, &array);
    glTexImage2D((GLenum)target, (GLint)level, (GLint)internalformat, (GLsizei)width, (GLsizei)height, (GLint)border, (GLenum)format, (GLenum)type, pixels);
    release_nio_buffer(env, array_ref, array);
}

JNIEXPORT void JNICALL Java_android_opengl_GLES20_glTexSubImage2D(JNIEnv *env, jclass, jint target, jint level, jint xoffset, jint yoffset, jint width, jint height, jint format, jint type, jobject pixels_buf) {
    jarray array_ref;
    jbyte *array;
    GLvoid *pixels = get_nio_buffer(env, pixels_buf, &array_ref, &array);
    glTexSubImage2D((GLenum)target, (GLint)level, (GLint)xoffset, (GLint)yoffset, (GLsizei)width, (GLsizei)height, (GLenum)format, (GLenum)type, pixels);
    release_nio_buffer(env, array_ref, array);
}

JNIEXPORT void JNICALL Java_android_opengl_GLES20_glTexParameterf(JNIEnv *env, jclass, jint target, jint pname, jfloat param) {
    glTexParameterf((GLenum)target, (GLenum)pname, (GLfloat)param);
}

JNIEXPORT void JNICALL Java_android_opengl_GLES20_glGenBuffers__I_3II(JNIEnv *env, jclass, jint n, jintArray buffers_ref, jint offset) {
    jint *buffers = (*env)->GetIntArrayElements(env, buffers_ref, NULL);
    glGenBuffers((GLsizei)n, (GLuint *)buffers + offset);
    (*env)->ReleaseIntArrayElements(env, buffers_ref, buffers, 0);
}

JNIEXPORT void JNICALL Java_android_opengl_GLES20_glBindBuffer(JNIEnv *env, jclass, jint target, jint buffer) {
    glBindBuffer((GLenum)target, (GLuint)buffer);
}

JNIEXPORT void JNICALL Java_android_opengl_GLES20_glBufferData(JNIEnv *env, jclass, jint target, jint size, jobject data_buf, jint usage) {
    jarray array_ref;
    jbyte *array;
    GLvoid *data = get_nio_buffer(env, data_buf, &array_ref, &array);
    glBufferData((GLenum)target, (GLsizeiptr)size, data, (GLenum)usage);
    release_nio_buffer(env, array_ref, array);
}

JNIEXPORT void JNICALL Java_android_opengl_GLES20_glDisableVertexAttribArray(JNIEnv *env, jclass, jint index) {
    glDisableVertexAttribArray((GLuint)index);
}

JNIEXPORT jint JNICALL Java_android_opengl_GLES20_glCreateShader(JNIEnv *env, jclass, jint type) {
    return (jint)glCreateShader((GLenum)type);
}

JNIEXPORT void JNICALL Java_android_opengl_GLES20_glShaderSource(JNIEnv *env, jclass, jint shader, jstring string) {
	const char* nativeString = (*env)->GetStringUTFChars(env, string, NULL);
    const char* strings[] = {nativeString};
    glShaderSource(shader, 1, strings, 0);
	(*env)->ReleaseStringUTFChars(env, string, nativeString);
}

JNIEXPORT void JNICALL Java_android_opengl_GLES20_glCompileShader(JNIEnv *env, jclass, jint shader) {
    glCompileShader((GLuint)shader);
}

JNIEXPORT void JNICALL Java_android_opengl_GLES20_glGetShaderiv__II_3II(JNIEnv *env, jclass, jint shader, jint pname, jintArray params_ref, jint offset) {
    jint *params = (*env)->GetIntArrayElements(env, params_ref, NULL);
    glGetShaderiv((GLuint)shader, (GLenum)pname, params + offset);
    (*env)->ReleaseIntArrayElements(env, params_ref, params, 0);
}

JNIEXPORT jint JNICALL Java_android_opengl_GLES20_glCreateProgram(JNIEnv *env, jclass) {
    return (jint)glCreateProgram();
}

JNIEXPORT void JNICALL Java_android_opengl_GLES20_glAttachShader(JNIEnv *env, jclass, jint program, jint shader) {
    glAttachShader((GLuint)program, (GLuint)shader);
}

JNIEXPORT void JNICALL Java_android_opengl_GLES20_glBindAttribLocation(JNIEnv *env, jclass, jint program, jint index, jstring name) {
	const char* nativeName = (*env)->GetStringUTFChars(env, name, NULL);
    glBindAttribLocation((GLuint)program, (GLuint)index, nativeName);
	(*env)->ReleaseStringUTFChars(env, name, nativeName);
}

JNIEXPORT void JNICALL Java_android_opengl_GLES20_glLinkProgram(JNIEnv *env, jclass, jint program) {
    glLinkProgram((GLuint)program);
}

JNIEXPORT void JNICALL Java_android_opengl_GLES20_glGetProgramiv__II_3II(JNIEnv *env, jclass, jint program, jint pname, jintArray params_ref, jint offset) {
    jint *params = (*env)->GetIntArrayElements(env, params_ref, NULL);
    glGetProgramiv((GLuint)program, (GLenum)pname, params + offset);
    (*env)->ReleaseIntArrayElements(env, params_ref, params, 0);
}

JNIEXPORT void JNICALL Java_android_opengl_GLES20_glGetActiveAttrib__III_3II_3II_3II_3BI(JNIEnv *env, jclass, jint program, jint index, jint bufsize, jintArray length_ref, jint lengthOffset, jintArray size_ref, jint sizeOffset, jintArray type_ref, jint typeOffset, jbyteArray name_ref, jint nameOffset) {
    jint *length = (*env)->GetIntArrayElements(env, length_ref, NULL);
    jint *size = (*env)->GetIntArrayElements(env, size_ref, NULL);
    jint *type = (*env)->GetIntArrayElements(env, type_ref, NULL);
    jbyte *name = (*env)->GetByteArrayElements(env, name_ref, NULL);
    glGetActiveAttrib((GLuint)program, (GLuint)index, (GLsizei)bufsize, (GLsizei *)length+lengthOffset, (GLint *)size+sizeOffset, (GLenum *)type+typeOffset, (char *)name+nameOffset);
    (*env)->ReleaseByteArrayElements(env, name_ref, name, 0);
    (*env)->ReleaseIntArrayElements(env, type_ref, type, 0);
    (*env)->ReleaseIntArrayElements(env, size_ref, size, 0);
    (*env)->ReleaseIntArrayElements(env, length_ref, length, 0);
}

JNIEXPORT jint JNICALL Java_android_opengl_GLES20_glGetAttribLocation(JNIEnv *env, jclass, jint program, jstring name) {
	const char* nativeName = (*env)->GetStringUTFChars(env, name, NULL);
    jint ret = glGetAttribLocation((GLuint)program, nativeName);
	(*env)->ReleaseStringUTFChars(env, name, nativeName);
    return ret;
}

JNIEXPORT void JNICALL Java_android_opengl_GLES20_glGetActiveUniform__III_3II_3II_3II_3BI(JNIEnv *env, jclass, jint program, jint index, jint bufsize, jintArray length_ref, jint lengthOffset, jintArray size_ref, jint sizeOffset, jintArray type_ref, jint typeOffset, jbyteArray name_ref, jint nameOffset) {
    jint *length = (*env)->GetIntArrayElements(env, length_ref, NULL);
    jint *size = (*env)->GetIntArrayElements(env, size_ref, NULL);
    jint *type = (*env)->GetIntArrayElements(env, type_ref, NULL);
    jbyte *name = (*env)->GetByteArrayElements(env, name_ref, NULL);
    glGetActiveUniform((GLuint)program, (GLuint)index, (GLsizei)bufsize, (GLsizei *)length+lengthOffset, (GLint *)size+sizeOffset, (GLenum *)type+typeOffset, (char *)name+nameOffset);
    (*env)->ReleaseByteArrayElements(env, name_ref, name, 0);
    (*env)->ReleaseIntArrayElements(env, type_ref, type, 0);
    (*env)->ReleaseIntArrayElements(env, size_ref, size, 0);
    (*env)->ReleaseIntArrayElements(env, length_ref, length, 0);
}

JNIEXPORT jint JNICALL Java_android_opengl_GLES20_glGetUniformLocation(JNIEnv *env, jclass, jint program, jstring name) {
	const char* nativeName = (*env)->GetStringUTFChars(env, name, NULL);
    jint ret = glGetUniformLocation((GLuint)program, nativeName);
	(*env)->ReleaseStringUTFChars(env, name, nativeName);
    return ret;
}

JNIEXPORT void JNICALL Java_android_opengl_GLES20_glDeleteShader(JNIEnv *env, jclass, jint shader) {
    glDeleteShader((GLuint)shader);
}

JNIEXPORT void JNICALL Java_android_opengl_GLES20_glUseProgram(JNIEnv *env, jclass, jint program) {
    glUseProgram((GLuint)program);
}

JNIEXPORT void JNICALL Java_android_opengl_GLES20_glVertexAttribPointer(JNIEnv *env, jclass, jint indx, jint size, jint type, jboolean normalized, jint stride, jint offset) {
    glVertexAttribPointer((GLuint)indx, (GLint)size, (GLenum)type, (GLboolean)normalized, (GLsizei)stride, (GLvoid *)(intptr_t)offset);
}

JNIEXPORT void JNICALL Java_android_opengl_GLES20_glUniformMatrix4fv__IIZ_3FI(JNIEnv *env, jclass, jint location, jint count, jboolean transpose, jfloatArray value_ref, jint offset) {
    jfloat *value = (*env)->GetFloatArrayElements(env, value_ref, NULL);
    glUniformMatrix4fv((GLint)location, (GLsizei)count, (GLboolean)transpose, (GLfloat *)value);
    (*env)->ReleaseFloatArrayElements(env, value_ref, value, 0);
}

JNIEXPORT void JNICALL Java_android_opengl_GLES20_glUniform1i(JNIEnv *env, jclass, jint location, jint x) {
    glUniform1i((GLint)location, (GLint)x);
}

JNIEXPORT void JNICALL Java_android_opengl_GLES20_glDrawArrays(JNIEnv *env, jclass, jint mode, jint first, jint count) {
    glDrawArrays((GLenum)mode, (GLint)first, (GLsizei)count);
}

JNIEXPORT void JNICALL Java_android_opengl_GLES20_glClearColor(JNIEnv *env, jclass, jfloat red, jfloat green, jfloat blue, jfloat alpha) {
    glClearColor((GLclampf)red, (GLclampf)green, (GLclampf)blue, (GLclampf)alpha);
}

JNIEXPORT void JNICALL Java_android_opengl_GLES20_glClear(JNIEnv *env, jclass, jint mask) {
    glClear((GLbitfield)mask);
}

JNIEXPORT void JNICALL Java_android_opengl_GLES20_glBlendFunc(JNIEnv *env, jclass, jint sfactor, jint dfactor) {
    glBlendFunc((GLenum)sfactor, (GLenum)dfactor);
}
