#include <gtk/gtk.h>
#include <stdint.h>
#include <stdio.h>

#include "../defines.h"
#include "../util.h"

#include "WrapperWidget.h"
#include "AdapterView.h"

#include "../generated_headers/android_widget_AbsListView.h"

static void range_list_model_init(RangeListModel *list_model) {}
static void range_list_model_class_init(RangeListModelClass *class) {}

static guint range_list_model_get_n_items(GListModel *list_model)
{
	return (RANGE_LIST_MODEL(list_model))->n_items;
}

static gpointer range_list_model_get_item(GListModel *list_model, guint index)
{
	if (index >= RANGE_LIST_MODEL(list_model)->n_items)
		return NULL;
	RangeListItem *item = g_object_new(range_list_item_get_type(), NULL);
	item->model = RANGE_LIST_MODEL(list_model);
	return item;
}

static void range_list_model_model_init(GListModelInterface *iface)
{
	iface->get_n_items = range_list_model_get_n_items;
	iface->get_item_type = (GType (*)(GListModel *))range_list_item_get_type;
	iface->get_item = range_list_model_get_item;
}

G_DEFINE_TYPE_WITH_CODE(RangeListModel, range_list_model, G_TYPE_OBJECT,
		G_IMPLEMENT_INTERFACE(G_TYPE_LIST_MODEL, range_list_model_model_init))

static void range_list_item_class_init(RangeListItemClass *cls){}
static void range_list_item_init(RangeListItem *self){}
G_DEFINE_TYPE(RangeListItem, range_list_item, G_TYPE_OBJECT)

static void on_click(GtkGestureClick *gesture, int n_press, double x, double y, GtkListItem *list_item)
{
	JNIEnv *env = get_jni_env();
	guint position = gtk_list_item_get_position(list_item);
	RangeListModel *model = RANGE_LIST_ITEM(gtk_list_item_get_item(list_item))->model;
	jobject listener = g_object_get_data(G_OBJECT(model->list_view), "on_click_listener");
	if (!listener)
		return;
	WrapperWidget *wrapper = WRAPPER_WIDGET(gtk_list_item_get_child(list_item));

	jmethodID onClick = _METHOD(_CLASS(listener), "onItemClick", "(Landroid/widget/AdapterView;Landroid/view/View;IJ)V");
	(*env)->CallVoidMethod(env, listener, onClick, model->jobject, wrapper->jobj, position, 0);

	if((*env)->ExceptionCheck(env))
		(*env)->ExceptionDescribe(env);
}

static void bind_listitem_cb(GtkListItemFactory *factory, GtkListItem *list_item)
{
	JNIEnv *env = get_jni_env();

	guint index = gtk_list_item_get_position(list_item);
	WrapperWidget *wrapper = WRAPPER_WIDGET(gtk_list_item_get_child(list_item));
	RangeListModel *model = RANGE_LIST_ITEM(gtk_list_item_get_item(list_item))->model;
	int n_items = g_list_model_get_n_items(G_LIST_MODEL(model));
	if (index >= n_items) {
		printf("invalid index: %d >= %d\n", index, n_items);
		exit(0);
	}
	jmethodID getView = _METHOD(_CLASS(model->adapter), "getView", "(ILandroid/view/View;Landroid/view/ViewGroup;)Landroid/view/View;");
	jobject view = (*env)->CallObjectMethod(env, model->adapter, getView, index, wrapper ? wrapper->jobj : NULL, model->jobject);
	view = _REF(view);
	GtkWidget *child = gtk_widget_get_parent(GTK_WIDGET(_PTR(_GET_LONG_FIELD(view, "widget"))));
	gtk_list_item_set_child(list_item, child);

	GtkEventController *controller = GTK_EVENT_CONTROLLER(gtk_gesture_click_new());
	gtk_event_controller_set_propagation_phase(controller, GTK_PHASE_BUBBLE);
	g_signal_connect(controller, "released", G_CALLBACK(on_click), list_item);
	gtk_widget_add_controller(GTK_WIDGET(child), controller);
	g_object_set_data(G_OBJECT(child), "on_item_click_listener", controller);
}

static void unbind_listitem_cb(GtkListItemFactory *factory, GtkListItem *list_item)
{
	GtkWidget *child = gtk_list_item_get_child(list_item);
	GtkEventController *controller = g_object_get_data(G_OBJECT(child), "on_item_click_listener");
	gtk_widget_remove_controller(child, controller);
}

JNIEXPORT jlong JNICALL Java_android_widget_AbsListView_native_1constructor(JNIEnv *env, jobject this, jobject context, jobject attrs)
{
	GtkWidget *wrapper = g_object_ref(wrapper_widget_new());

	GtkListItemFactory *factory = gtk_signal_list_item_factory_new();
	g_signal_connect(factory, "bind", G_CALLBACK(bind_listitem_cb), NULL);
	g_signal_connect(factory, "unbind", G_CALLBACK(unbind_listitem_cb), NULL);
	RangeListModel *model = g_object_new(range_list_model_get_type(), NULL);
	GtkWidget *list_view = gtk_list_view_new(GTK_SELECTION_MODEL(gtk_single_selection_new(G_LIST_MODEL(model))), factory);
	model->list_view = list_view;
	model->jobject = _REF(this);
	GtkWidget *scrolled_window = gtk_scrolled_window_new();
	gtk_scrolled_window_set_propagate_natural_height(GTK_SCROLLED_WINDOW(scrolled_window), TRUE);
	gtk_scrolled_window_set_propagate_natural_width(GTK_SCROLLED_WINDOW(scrolled_window), TRUE);
	gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scrolled_window), list_view);
	wrapper_widget_set_child(WRAPPER_WIDGET(wrapper), scrolled_window);
	wrapper_widget_set_jobject(WRAPPER_WIDGET(wrapper), env, this);
	gtk_widget_set_name(list_view, "ListView");
	gtk_list_view_set_show_separators(GTK_LIST_VIEW(list_view), TRUE);
	return _INTPTR(scrolled_window);
}

JNIEXPORT void JNICALL Java_android_widget_AbsListView_native_1setAdapter(JNIEnv *env, jobject this, jlong widget_ptr, jobject adapter)
{
	GtkScrolledWindow *scrolled_window = GTK_SCROLLED_WINDOW(_PTR(widget_ptr));
	GtkListView *list_view = GTK_LIST_VIEW(gtk_scrolled_window_get_child(scrolled_window));
	RangeListModel *model = RANGE_LIST_MODEL(gtk_single_selection_get_model(GTK_SINGLE_SELECTION(gtk_list_view_get_model(list_view))));

	if (model->adapter)
		_UNREF(model->adapter);
	model->adapter = adapter ? _REF(adapter) : NULL;
	guint old_n_items = model->n_items;
	model->n_items = adapter ? (*env)->CallIntMethod(env, adapter, _METHOD(_CLASS(adapter), "getCount", "()I")) : 0;
	g_list_model_items_changed(G_LIST_MODEL(model), 0, old_n_items, model->n_items);
}

JNIEXPORT void JNICALL Java_android_widget_AbsListView_setItemChecked(JNIEnv *env, jobject this, jint position, jboolean checked)
{
	GtkScrolledWindow *scrolled_window = GTK_SCROLLED_WINDOW(_PTR(_GET_LONG_FIELD(this, "widget")));
	GtkListView *list_view = GTK_LIST_VIEW(gtk_scrolled_window_get_child(scrolled_window));
	GtkSelectionModel *model = gtk_list_view_get_model(list_view);

	if (checked)
		gtk_selection_model_select_item(model, position, FALSE);
	else
		gtk_selection_model_unselect_item(model, position);
}

static void on_activate(GtkListView *list_view, guint position)
{
	JNIEnv *env = get_jni_env();
	jobject listener = g_object_get_data(G_OBJECT(list_view), "on_click_listener");
	RangeListModel *model = RANGE_LIST_MODEL(gtk_single_selection_get_model(GTK_SINGLE_SELECTION(gtk_list_view_get_model(list_view))));
	jmethodID onClick = _METHOD(_CLASS(listener), "onItemClick", "(Landroid/widget/AdapterView;Landroid/view/View;IJ)V");
	(*env)->CallVoidMethod(env, listener, onClick, model->jobject, NULL, position, 0);
}

JNIEXPORT void JNICALL Java_android_widget_AbsListView_setOnItemClickListener(JNIEnv *env, jobject this, jobject listener)
{
	GtkScrolledWindow *scrolled_window = GTK_SCROLLED_WINDOW(_PTR(_GET_LONG_FIELD(this, "widget")));
	GtkListView *list_view = GTK_LIST_VIEW(gtk_scrolled_window_get_child(scrolled_window));
	g_object_set_data(G_OBJECT(list_view), "on_click_listener", _REF(listener));

	g_signal_connect(list_view, "activate", G_CALLBACK(on_activate), NULL);
}

JNIEXPORT jint JNICALL Java_android_widget_AbsListView_getCheckedItemPosition(JNIEnv *env, jobject this)
{
	GtkScrolledWindow *scrolled_window = GTK_SCROLLED_WINDOW(_PTR(_GET_LONG_FIELD(this, "widget")));
	GtkListView *list_view = GTK_LIST_VIEW(gtk_scrolled_window_get_child(scrolled_window));
	GtkSingleSelection *single_selection = GTK_SINGLE_SELECTION(gtk_list_view_get_model(list_view));

	return gtk_single_selection_get_selected(single_selection);
}
