### PoC for a translation layer that would execute the abolute minimum of android-specific code necessary in order to run an android app on a Linux system

---

dependencies on debian:
`sudo apt install libglib2.0-dev libgtk-4-dev libasound2-dev libopenxr-dev`

build instructions:  
1. compile and install https://gitlab.com/Mis012/dalvik_standalone (art branch) (you can run it from builddir, but it requires some additional env variables)
2. `mkdir builddir`
3. `meson builddir`
4. `ninja -C builddir`

then, to run from builddir:  
`cd builddir`  
and  
`RUN_FROM_BUILDDIR= LD_LIBRARY_PATH=./ ANDROID_APP_DATA_DIR=../test_apks/example_data_dir/ ./android-translation-layer ../test_apks/org.happysanta.gd_29.apk -l org/happysanta/gd/GDActivity`  
(for an example of a full game working that can be distributed along this)
or  
`RUN_FROM_BUILDDIR= LD_LIBRARY_PATH=./ ANDROID_APP_DATA_DIR=../test_apks/example_data_dir/ ./android-translation-layer ../test_apks/gles3jni.apk -l com/android/gles3jni/GLES3JNIActivity`  
(for a sample app using OpenGL from native code to do it's rendering) (note: the lib in `test_apks/example_data_dir/gles3jni.apk/lib/` is for x86_64, make sure you extract the lib for your architecture from the apk)  

to install:  
`meson install`  

to run after installataion:  
`ANDROID_APP_DATA_DIR=[data dir] android-translation-layer [path to apk] -l [activity to launch]`  
or just  
`android-translation-layer [path to apk] -l [activity to launch]`  
to use the default data dir of `~/.local/share/android_translation_layer/`

NOTE: some apps don't like runtime changes to resolution, and currently GLSurfaceView will stretch instead of changing resolution  
to sidestep this, we allow for specifying the initial resolution, which will currently always get passed as the screen resolution to the app and to GLSurfaceView even when you resize the window.
example with custom width/height: `android-translation-layer path/to/org.happysanta.gd_29.apk -l org/happysanta/gd/GDActivity -w 540 -h 960`

NOTE: you might need to copy some files out from the apk under `ANDROID_APP_DATA_DIR` 
(defaults to `~/.local/share/android_translation_layer/`), e.g the `assets` folder;  
NOTE: on X11, Gtk might decide to use GLX, which completely messes up our EGL-dependent code.
If you have a debug build of Gtk, you can use GDK_DEBUG=gl-egl to force the use of EGL  
NOTE: we don't currently handle signed apks; simply remove the META-INF folder from an apk to skip signature verification

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

##### Roadmap:

- fix issues mention above

- fix ugly hacks
	- don't assume apps only have one (relevant) activity
	- figure out a way to dynamically change the resolution of a GLSurfaceView

- implement more stuff (there is a lot of it, and it won't get done if nobody helps... ideally pick a simple-ish application and stub/implement stuff until it works)

- explore using bubblewrap to enforce the security policies that google helpfully forces apps to comply with (and our own security policies, like no internet access for apps which really shouldn't need it and are not scummy enough to refuse to launch without it)

##### Tips:

- the correct format for changing verbosity of messages going through android's logging library is `ANDROID_LOG_TAGS=*:v` (where `*` is "all tags" and `v` is "verbosity `verbose` or lesser"
