#include <gtk/gtk.h>
#include <webkit/webkit.h>

#include "../defines.h"
#include "../util.h"

#include "WrapperWidget.h"

#include "../generated_headers/android_view_View.h"
#include "../generated_headers/android_webkit_WebView.h"

JNIEXPORT jlong JNICALL Java_android_webkit_WebView_native_1constructor(JNIEnv *env, jobject this, jobject context, jobject attrs)
{
	/* 
         * many apps use webview just for fingerprinting or displaying ads, which seems like
         * a waste of resources even if we deal with fingerprinting and ads in some other way
         * in the future.
         */
	if(!getenv("ATL_UGLY_ENABLE_WEBVIEW"))
		return Java_android_view_View_native_1constructor(env, this, context, attrs);

	GtkWidget *wrapper = g_object_ref(wrapper_widget_new());
	GtkWidget *webview = webkit_web_view_new();
	wrapper_widget_set_child(WRAPPER_WIDGET(wrapper), webview);
	wrapper_widget_set_jobject(WRAPPER_WIDGET(wrapper), env, this);
	return _INTPTR(webview);
}

JNIEXPORT void JNICALL Java_android_webkit_WebView_native_1loadUrl(JNIEnv *env, jobject this, jlong widget_ptr, jstring url)
{
	if(!getenv("ATL_UGLY_ENABLE_WEBVIEW"))
		return;

	WebKitWebView *webview = _PTR(widget_ptr);
	webkit_web_view_load_uri(webview, _CSTRING(url));
}

JNIEXPORT void JNICALL Java_android_webkit_WebView_native_1loadDataWithBaseURL(JNIEnv *env, jobject this, jlong widget_ptr, jstring base_url, jstring data_jstr, jstring mime_type, jstring encoding)
{
	if(!getenv("ATL_UGLY_ENABLE_WEBVIEW"))
		return;

	WebKitWebView *webview = _PTR(widget_ptr);
	jsize data_len = (*env)->GetStringUTFLength(env, data_jstr);
	jsize data_jlen = (*env)->GetStringLength(env, data_jstr);
	char *data = malloc(data_len + 1); // + 1 for NUL
	(*env)->GetStringUTFRegion(env, data_jstr, 0, data_jlen, data);
	webkit_web_view_load_bytes(webview, g_bytes_new(data, data_len), _CSTRING(mime_type), _CSTRING(encoding), _CSTRING(base_url));
}
