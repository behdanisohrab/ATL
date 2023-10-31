#ifndef ANDROID_LAYOUT_H
#define ANDROID_LAYOUT_H

#include <jni.h>
#include <gtk/gtk.h>

G_DECLARE_FINAL_TYPE(AndroidLayout, android_layout, ATL, ANDROID_LAYOUT, GtkLayoutManager);

struct _AndroidLayout {
	GtkLayoutManager parent_instance;
	jobject view;
};

GtkLayoutManager *android_layout_new(jobject view);

#endif  // ANDROID_LAYOUT_H