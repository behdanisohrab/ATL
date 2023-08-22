#include <gtk/gtk.h>
#include <jni.h>

#include "../defines.h"
#include "../generated_headers/android_app_AlertDialog.h"

JNIEXPORT jlong JNICALL Java_android_app_AlertDialog_nativeInit(JNIEnv *env, jobject this)
{
	GtkWidget *dialog = gtk_dialog_new();
	g_signal_connect_swapped(dialog, "response", G_CALLBACK(gtk_window_destroy), dialog);
	return _INTPTR(dialog);
}

JNIEXPORT void JNICALL Java_android_app_AlertDialog_nativeSetTitle(JNIEnv *env, jobject this, jlong ptr, jstring title)
{
	GtkWindow *dialog = GTK_WINDOW(_PTR(ptr));
	const char* nativeTitle = (*env)->GetStringUTFChars(env, title, NULL);
	gtk_window_set_title(dialog, nativeTitle);
	(*env)->ReleaseStringUTFChars(env, title, nativeTitle);
}

JNIEXPORT void JNICALL Java_android_app_AlertDialog_nativeSetMessage(JNIEnv *env, jobject this, jlong ptr, jstring message)
{
	GtkDialog *dialog = GTK_DIALOG(_PTR(ptr));
	const char* nativeMessage = (*env)->GetStringUTFChars(env, message, NULL);
	GtkWidget *content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
	GtkWidget *label = gtk_label_new(nativeMessage);
	gtk_label_set_wrap(GTK_LABEL(label), TRUE);
	gtk_box_append(GTK_BOX(content_area), label);
	(*env)->ReleaseStringUTFChars(env, message, nativeMessage);
}

JNIEXPORT void JNICALL Java_android_app_AlertDialog_nativeSetButton(JNIEnv *env, jobject this, jlong ptr, jint id, jstring text) {
	GtkDialog *dialog = GTK_DIALOG(_PTR(ptr));
	const char* nativeText = (*env)->GetStringUTFChars(env, text, NULL);
	gtk_dialog_add_button(dialog, nativeText, id);
	(*env)->ReleaseStringUTFChars(env, text, nativeText);
}

struct click_callback_data {
	JavaVM *jvm;
	jobject this;
	jobject on_click_listener;
	jmethodID on_click_method;
};

struct _ListEntry {
  GObject parent;
  const char *text;
};
G_DECLARE_FINAL_TYPE(ListEntry, list_entry, ATL, LIST_ENTRY, GObject);
static void list_entry_class_init(ListEntryClass *cls){}
static void list_entry_init(ListEntry *self){}
G_DEFINE_TYPE(ListEntry, list_entry, G_TYPE_OBJECT)

static void setup_listitem_cb(GtkListItemFactory *factory, GtkListItem *list_item)
{
	gtk_list_item_set_child(list_item, gtk_label_new(""));
}

static void bind_listitem_cb(GtkListItemFactory *factory, GtkListItem *list_item)
{
	GtkWidget *label = gtk_list_item_get_child(list_item);
	ListEntry *entry = gtk_list_item_get_item(list_item);

	gtk_label_set_text(GTK_LABEL(label), entry->text);
}

static void activate_cb(GtkListView *list, guint position, struct click_callback_data *d)
{
	JNIEnv *env;
	(*d->jvm)->GetEnv(d->jvm, (void**)&env, JNI_VERSION_1_6);

	(*env)->CallVoidMethod(env, d->on_click_listener, d->on_click_method, d->this, position);
	if((*env)->ExceptionCheck(env))
		(*env)->ExceptionDescribe(env);
}

JNIEXPORT void JNICALL Java_android_app_AlertDialog_nativeSetItems(JNIEnv *env, jobject this, jlong ptr, jobjectArray items, jobject on_click) {
	GtkDialog *dialog = GTK_DIALOG(_PTR(ptr));

	GListStore *store = g_list_store_new(list_entry_get_type());
	int stringCount = (*env)->GetArrayLength(env, items);
	for (int i=0; i<stringCount; i++) {
		ListEntry *entry = ATL_LIST_ENTRY(g_object_new(list_entry_get_type(), NULL));
		entry->text = _CSTRING((*env)->GetObjectArrayElement(env, items, i));
		g_list_store_append(store, entry);
	}

	GtkListItemFactory *factory = gtk_signal_list_item_factory_new();
	g_signal_connect(factory, "setup", G_CALLBACK(setup_listitem_cb), NULL);
	g_signal_connect(factory, "bind", G_CALLBACK(bind_listitem_cb), NULL);
	GtkWidget *list = gtk_list_view_new(GTK_SELECTION_MODEL(gtk_single_selection_new(G_LIST_MODEL(store))), factory);
	gtk_list_view_set_single_click_activate(GTK_LIST_VIEW(list), TRUE);

	GtkWidget *content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
	gtk_box_append(GTK_BOX(content_area), list);

	JavaVM *jvm;
	(*env)->GetJavaVM(env, &jvm);

	struct click_callback_data *callback_data = malloc(sizeof(struct click_callback_data));
	callback_data->jvm = jvm;
	callback_data->this = _REF(this);
	callback_data->on_click_listener = _REF(on_click);
	callback_data->on_click_method = _METHOD(_CLASS(on_click), "onClick", "(Landroid/content/DialogInterface;I)V");

	g_signal_connect(list, "activate", G_CALLBACK(activate_cb), callback_data);
}

JNIEXPORT void JNICALL Java_android_app_AlertDialog_nativeShow(JNIEnv *env, jobject this, jlong ptr)
{
	GtkWindow *dialog = GTK_WINDOW(_PTR(ptr));
	gtk_window_present(dialog);
}
