A translation layer that allows running Android apps on a Linux system

![Angry Birds 3.2.0, Worms 2 Armageddon, and Gravity Defied running side by side by side](https://gitlab.com/android_translation_layer/android_translation_layer/-/raw/master/screenshot_2.png)

### Build
see [build documentation](https://gitlab.com/android_translation_layer/android_translation_layer/-/blob/master/doc/Build.md)

### Run in builddir
```sh
cd builddir
```
For an example of a full game working that can be distributed along this:
```sh
RUN_FROM_BUILDDIR= LD_LIBRARY_PATH=./ ./android-translation-layer /path/to/test_apks/org.happysanta.gd_29.apk -l org/happysanta/gd/GDActivity
```
Or for a sample app using OpenGL from native code to do it's rendering:
```sh
RUN_FROM_BUILDDIR= LD_LIBRARY_PATH=./ ./android-translation-layer path/to/test_apks/gles3jni.apk -l com/android/gles3jni/GLES3JNIActivity
```
Note: the test apks are available at https://gitlab.com/android_translation_layer/atl_test_apks.

### Run after installation
```sh
cd builddir
meson install
```

To run with the default data dir `~/.local/share/android_translation_layer/`:
```sh
android-translation-layer [path to apk] [-l activity to launch]
```
For custom data dir:
```sh
ANDROID_APP_DATA_DIR=[data dir] android-translation-layer [path to apk] [-l activity to launch]
```

### Tweaks
##### Resolution Changes
Some apps don't like runtime changes to resolution. To sidestep this, we allow for specifying the initial resolution.
example with custom width/height:
```sh
android-translation-layer path/to/org.happysanta.gd_29.apk -l org/happysanta/gd/GDActivity -w 540 -h 960
```

#### GLX on X11
On X11, Gtk might decide to use GLX, which completely messes up our EGL-dependent code.
Use GDK_DEBUG=gl-egl to force the use of EGL.  

### Contribute
If you are trying to launch a random app, chances are that we are missing implementations for some  
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

### Roadmap

- fix issues mentioned above

- fix ugly hacks

- implement more stuff (there is a lot of it, and it won't get done if nobody helps... ideally pick a simple-ish application and stub/implement stuff until it works)

- explore using bubblewrap to enforce the security policies that google helpfully forces apps to comply with (and our own security policies, like no internet access for apps which really shouldn't need it and are not scummy enough to refuse to launch without it)

### Tips

- the correct format for changing verbosity of messages going through android's logging library is `ANDROID_LOG_TAGS=*:v` (where `*` is "all tags" and `v` is "verbosity `verbose` or lesser"  
(note that specifying anything other than `*` as the tag will not work with the host version of liblog)
