### A translation layer that allows running android apps on a Linux system

---

dependencies on debian:
`sudo apt install libglib2.0-dev libgtk-4-dev libasound2-dev libopenxr-dev libportal-dev libsqlite3-dev`

dependencies you might have to build from source:
- https://github.com/Mis012/skia/tree/with-patches-applied  
(can also use the prebuilt in https://www.nuget.org/api/v2/package/SkiaSharp.NativeAssets.Linux/2.88.5 if it's compatible with your distro)

some apps depend on the following (though the translation layer itself currently does not):
- https://gitlab.com/Mis012/libopensles-standalone/

build instructions:  
1. compile and install https://gitlab.com/Mis012/dalvik_standalone (art branch) (you can run it from builddir, but it requires some additional env variables)
2. `mkdir builddir`
3. `meson builddir`
4. `ninja -C builddir`

then, to run from builddir:  
`cd builddir`  
and  
`RUN_FROM_BUILDDIR= LD_LIBRARY_PATH=./ ./android-translation-layer ../test_apks/org.happysanta.gd_29.apk -l org/happysanta/gd/GDActivity`  
(for an example of a full game working that can be distributed along this)
or  
`RUN_FROM_BUILDDIR= LD_LIBRARY_PATH=./ ./android-translation-layer ../test_apks/gles3jni.apk -l com/android/gles3jni/GLES3JNIActivity`  
(for a sample app using OpenGL from native code to do it's rendering)  

to install:  
`meson install`  

to run after installataion:  
`ANDROID_APP_DATA_DIR=[data dir] android-translation-layer [path to apk] -l [activity to launch]`  
or just  
`android-translation-layer [path to apk] -l [activity to launch]`  
to use the default data dir of `~/.local/share/android_translation_layer/`

NOTE: some apps don't like runtime changes to resolution.  
to sidestep this, we allow for specifying the initial resolution.
example with custom width/height: `android-translation-layer path/to/org.happysanta.gd_29.apk -l org/happysanta/gd/GDActivity -w 540 -h 960`

NOTE: on X11, Gtk might decide to use GLX, which completely messes up our EGL-dependent code.
Use GDK_DEBUG=gl-egl to force the use of EGL.  

when it doesn't work:  
if you are trying to launch a random app, chances are that we are missing implementations for some  
stuff that it needs, and we also don't have (sufficiently real looking) stubs for the stuff it says  
it needs but doesn't really.  
the workflow is basically to see where it fails (usually a Class or Method was not found) and to create  
stubs which sufficiently satisfy the app so that it continues trying to launch.  
once the app launches, you may find that some functionality (UI elements, ...) is missing; to enable  
such functionality, you need to convert the relevant stubs to actual implementation.  
you can look at simple widgets (e.g. TextView, or ImageView) to see how to implement a widget such that  
it shows up as a Gtk Widget.  

for more specific instructions, see `doc/QuickHelp.md`
for general description of the architecure, see `doc/Architecture.md`

if you want to contribute, and find the codebase overwhelming, don't hesitate to open an issue
so we can help you out and possibly write more documentation.

screenshot:

![angry birds 3.2.0, Worms 2 Armageddon, and gravity defied running side by side by side](https://gitlab.com/Mis012/android_translation_layer_PoC/-/raw/master/screenshot_2.png)

##### Roadmap:

- fix issues mentioned above

- fix ugly hacks

- implement more stuff (there is a lot of it, and it won't get done if nobody helps... ideally pick a simple-ish application and stub/implement stuff until it works)

- explore using bubblewrap to enforce the security policies that google helpfully forces apps to comply with (and our own security policies, like no internet access for apps which really shouldn't need it and are not scummy enough to refuse to launch without it)

##### Tips:

- the correct format for changing verbosity of messages going through android's logging library is `ANDROID_LOG_TAGS=*:v` (where `*` is "all tags" and `v` is "verbosity `verbose` or lesser"  
(note that specifying anything other than `*` as the tag will not work with the host version of liblog)
