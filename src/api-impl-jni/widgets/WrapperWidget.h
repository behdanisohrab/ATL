#ifndef WRAPPER_WIDGET_H
#define WRAPPER_WIDGET_H

G_DECLARE_FINAL_TYPE (WrapperWidget, wrapper_widget, WRAPPER, WIDGET, GtkWidget)

struct _WrapperWidget
{
	GtkWidget parent_instance;
};

struct _WrapperWidgetClass
{
	GtkWidgetClass parent_class;
};

GtkWidget * wrapper_widget_new(void);
void wrapper_widget_set_child(WrapperWidget *parent, GtkWidget *child);

#endif
