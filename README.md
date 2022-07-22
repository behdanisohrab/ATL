### PoC for a translation layer that would execute the abolute minimum of android-specific code necessary in order to run an android app on a Linux system

---

build instructions:  
`make`: compile everything (except dalvik, which is precompiled from https://gitlab.com/Mis012/dalvik_standalone)  

usage:  
`./launch_activity.sh <path_to_apk> <path_to_activity>`  
example: `./launch_activity.sh test_apks/org.happysanta.gd_29.apk org/happysanta/gd/GDActivity`  
note: some apps don't like runtime changes to resolution, and currently GLSurfaceView will stretch instead of changing resolution  
example with custom width/height: `./launch_activity.sh test_apks/org.happysanta.gd_29.apk 'org/happysanta/gd/GDActivity -w 540 -h 960'`

NOTE: you might need to copy some files out from the apk under `data/`, e.g the `assets` folder
NOTE: on X11, Gtk might decide to use GLX, which completely messes up our EGL-dependent code.
If you have a debug build of Gtk, you can use GDK_DEBUG=gl-egl to force the use of EGL

when it doesn't work:  
if you are trying to launch a random app, chances are that we are missing implementations for some  
stuff that it needs, and we also don't have (sufficiently real looking) stubs for the stuff it says  
it needs but doesn't really.  
the workflow is basically to see where it fails (usually a Class or Method was not found) and to create  
stubs which sufficiently satisfy the app so that it continues trying to launch.  
once the app launches, you may find that some functionality (UI elements, ...) is missing; to enable  
such functionality, you need to convert the relevant stubs to actual implementation.  
you can look at simple widgets (e.g. TextView, or ImageView which is extremely simple since it's  
currently displaying an image-not-found icon instead of the actual image) to see how to implement a  
widget such that it shows up as a Gtk Widget. for layout widgets, you can cheat by duplicating  
LinearLayout or RelativeLayout, which are currently implemented in an extremely simplistic manner  
(it might not look correctly, but it should *work*)

for more specific instructions, see `doc/QuickHelp.md`
for general description of the architecure, see `doc/Architecture.md`

screenshot:

![angry birds 3.2.0, Worms 2 Armageddon, and gravity defied running side by side by side](https://gitlab.com/Mis012/android_translation_layer_PoC/-/raw/master/screenshot_2.png)

note: running two or more different apps at the same time needs a tiny bit of luck, since the data directory path is currently hardcoded to `./data/` no matter the app  
note: the GLSurfaceView widgets currently start out with zero width, which is somewhat helpful for debugging so it wasn't fixed yet; to get the app to start rendering, enable hexpand on the Wrapper widget around the GtkGLArea using Gtk Inspector

##### FAQ:

Q:  
	did you ever notice that most apps nowadays use at least some .so libraries?  
	some apps even use them for 99% of their functionality!  

A:  
	recently, support for loading bionic-linked libraries was added, and two games which use  
	GLSurfaceView (rendered to from native code) as their main UI element - Angry Birds 3.2.0  
	and Worms 2 Armageddon - are currently fully playable

Q:  
	this seems pretty basic, most apps don't work at all...  
A:  
	we have three apps working for now, how fast that number grows depends on how many  
	people work on reimplementing stuff :)  
	there are probably more than enough people in the world with the required skills who don't have  
	anything better to do for the number of implemented APIs to grow appreciably fast, maybe you are  
	one of them? ;)

Q:  
	ok, so this can sorta run three apps. What's the catch?  

A:  
	well, first things first, technically I compiled Gravity Defied myself and removed  
	some bug-reporting-init-related code which I got frustrated with stubbing out. however,  
	adding more stubs should make that unnecessary.  
	now for the second issue: `./data/` contains some stuff that should instead be read from the apk,  
	and some of this stuff is also externally converted (e.g Xml from binary form to actual Xml).  
	obviously this is not ideal for user experience. NOTE: it seems that the binary form *might* be  
	protobuf-based, which would make reading it directly easier.  
	and the third issue: Gravity Defied is still extremely simple compared to most android apps,  
	doesn't acknowledge compat layers, and the most intricate UI element is completely custom drawn  
	using the canvas interface, in a manner that makes it easy to implement with cairo.
	angry birds (old version) and worms 2 armageddon were chosen because they similarly don't use  
	compat layers, and basically the entire UI is custom drawn with OpenGL calls from native code.

##### Roadmap:

- fix issues mention above

- fix ugly hacks
	- don't assume apps only have one (relevant) activity
	- figure out a way to dynamically change the resolution of a GLSurfaceView

- implement more stuff (there is a lot of it, and it won't get done if nobody helps... ideally pick a simple-ish application and stub/implement stuff until it works)

- implement the alternatives to GLSurfaceView (using SurfaceView to get an EGL surface, native activity, not sure if there are others?)
