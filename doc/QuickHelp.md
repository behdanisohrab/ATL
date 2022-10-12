#### quick guide to implementing as-of-yet uinmplemented android APIs to make a new app work

If you try to run any random android app using the translation layer, you will most likely encounter errors 
telling you that some class/interface/function doesn't exist. If you wish to help improve the translation layer 
by adding support for the missing APIs (and if not you, who will), then read ahead :)

##### simplest case: stubbing

In a lot of cases, the functionality of the missing APIs is not really relevant to you. And even when it is, 
you would probably prefer to first get the app to launch without errors. With a lot of APIs, you can get away 
with writing stubs.

What is a stub? Well, the simplest stub would be an empty class, like this:
`src/api-impl/android/webkit/WebView.java:`
```Java
package android.webkit;

public class WebView {
}
```
This should fix the "no such class" error, and let you get further in your attempt to simply launch the app.  
If the app uses a non-default constructor, you will need to provide that as well (empty is fine), and you will 
need to provide stub classes for any types used for paramters.

Since the class was needed, it's pretty likely that up next you will get a "no such method" error. The easiest 
case is a void method:
```Java
package android.webkit;

import android.content.Context;

public class WebView {
	public void doSomething(Context context) {
	}
}
```
here, all that you need to take care of is that at `src/api-impl/android/content/Context.java`, you have at minimum 
a stub class.

Unfortunately, in the WebView case, the method that an app was trying call wasn't returning `void`. If this is 
the case, your best case scenario is that you can return some sort of value that will make you progress further. 
For example, if the method is called DoWeHaveInternetConnection, it's pretty likely that upon decompiling 
the app, you will find that returing `false` from that function makes the app decide not to attempt to use 
Internet-related APIs (which you might not feel like implementing at the moment).  
Sadly, in our case, the return type is an Object. If that's the case, and the Object is of a type not yet 
implemented, you can try simply making a stub class for said object, and then returning `null`.

```Java
package android.webkit;

import android.content.Context;

// the only reason we need to implement this is that some app developers are such scumbags that they try to use this for tracking purposes
public class WebView {
	public WebView (Context context) {
	}

	public WebSettings getSettings() {
		return null;
	}
}

```
this will obviously only work when the app checks for NULL, and decides to abandon whatever it was planning to 
do when NULL is returned. (and you don't mind that it does so)

When that's not an option, simply return a new instance of the stub class:
```Java
package android.webkit;

import android.content.Context;

// the only reason we need to implement this is that some app developers are such scumbags that they try to use this for tracking purposes
public class WebView {
	public WebView (Context context) {
	}

	public WebSettings getSettings() {
		return new WebSettings();
	}
}
```

This might be enough, but quite often, the returned Object's methods are called by the app. If that's the case, 
simply create stub methods in that class same as we did above, and after a few iterations you should get to the 
end of the rabbit hole.

NOTE: in some cases, such as with enums and interfaces, you should be able to simply copy the APACHE-licensed 
android code. With interfaces, you might want to comment out any methods not needed for the app you are trying 
to get to work in order to cut down on the amount of stubbing you need to do.

##### intriguing case: widgets

There are two basic types of widgets (Views): containers (Layouts) and the rest.

To implement a container widget, simply copy an existing container widget implementation (e.g LinearLayout 
(`src/api-impl/android/widget/LinearLayout.java` and `src/api-impl-jni/widgets/android_widget_LinearLayout.c`)), and that's 
it! Now, chances are that you wanted something slightly different, but this will at least display the child 
widgets so that you can focus on implementing those.

When you get around to properly implementing the particularities of the specific container widget, it's 
basically the same process as below.

To implement any other widget, copy a widget that is closest to what you're looking for, and if Gtk has 
a better approximation for your widget, then change to that as the backing Gtk widget. If Gtk doesn't have 
anything close enough, you will need to implement your own widget. You might need to do that anyway, and wrap 
the close-enough Gtk widget, since subclassing is mostly not possible in Gtk.

###### case study: ImageView

`src/api-impl/android/widget/ImageView.java`
```Java
package android.widget;
```
↑ most widgets are in this package, but not all of them are
```Java
import android.util.AttributeSet;
import android.content.Context;

import android.view.View;
```
↑ any widget will need to import these
```
public class ImageView extends View {
	public ImageView(AttributeSet attrs) {
		super(attrs);

		native_constructor(attrs);
	}

	public ImageView(Context context) {
		super(context);

		native_constructor(context);
	}

	private native void native_constructor(AttributeSet attrs);
	private native void native_constructor(Context context);
```
↑ at least these will be needed for any widget
```Java
	public /*native*/ void setImageResource(final int resid) {}
	public void setAdjustViewBounds(boolean adjustViewBounds) {}

	public void setScaleType(ScaleType scaleType) {}

    public enum ScaleType { ... }
}
```
↑ you might need some stubs, don't fall into the trap of thinking that you need to immediately implement these

---

`src/api-impl-jni/widgets/android_widget_ImageView.c`
```C
#include <gtk/gtk.h>

#include "../defines.h"
#include "../util.h"

#include "WrapperWidget.h"

```
↑ every widget will be under `src/api-impl-jni/widgets/` and will have these includes
```C
#include "../generated_headers/android_widget_ImageView.h"
```
↑ this is the jni-generated header file (btw, t's name is what dicates the name of this .c file)
↓ there should be two functions here, one for the `Context` costructor and one for the `AttributeSet` one; for start, you can keep them the same
```C
JNIEXPORT void JNICALL Java_android_widget_ImageView_native_1constructor__Landroid_content_Context_2(JNIEnv *env, jobject this, jobject context)
{
	GtkWidget *wrapper = wrapper_widget_new();
```
↑ the wrapper widget is required, it's expected by generic functions operating on widgets; the purpose is to allow for things like background image
handling for cases where we can't subclass the backing widget itself
```C
	GtkWidget *image = gtk_image_new_from_icon_name("FIXME"); // will not actually use gtk_image_new_from_icon_name when implementing this, but we want that nice "broken image" icon
```
↑ here we create the actual backing Gtk widget.
```C
	wrapper_widget_set_child(WRAPPER_WIDGET(wrapper), image);
```
↑ put the widget in the wrapper
```C
	_SET_LONG_FIELD(this, "widget", _INTPTR(image));
```
↑ set the `widget` member of the View-derived class to the pointer to our widget; this lets us access this widget in the context of the View, which is precisely when we will need to access it
```
}

```
