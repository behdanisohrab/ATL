### A translation layer that allows running android apps on a Linux system

---

#### what's this branch?

this branch is an experiment to see how large changes would be required in order to make
this project work with a "standard" JVM. There are not necessarily any benefits to doing this,
quite on the contrary, but it might be interesting to use GraalVM native image in order
to support "porting" of fully-(or mostly-)native apps, where a full JVM isn't really necessary
but we would still like to use most of this project as-is, with parts written in java and C code
using JNI.

There's also the remote possibility that someone will at some point make GraalVM support
dex bytecode, in which case we could avoid the maintenance burden of being a downstream of art
(though it seems quite unlikely that someone will not only introduce dex support to GraalVM but
also maintain it)

Tested on OpenJDK 17 and GraalVM 21 (also native-image, see below) with ndk endless tunnel sample
(fully native using NativeActivity) and Gravity Defied (fully java, but we have
the source code so why not try) (GD was not tested with native-image)

endless tunnel works fine, Gravity Defied may work fine with `apachehttp-host.jar` compiled against
non-art bootstrap but this wasn't tested.

some stuff had to be patched out because I didn't see a trivial way to make it work with !art,
some stuff was changed to what may or may not be equivalent code.

this is here just as a backup (and so that someone who independently thinks of trying this can save
themselves some sad duplicate work), it's not supported in any way.

basic instructions if you want to try this yourself (this is unsupported stuff, but might as well
try to help):

you need api-impl.jar and ARSCLib.jar, `javac -source 8 -target 8 **/*.java` in the right folder
and `jar -cvf <name>` seems to work alright (the versions compiled against art bootstrap try to use
stuff like `arraycopy` in a version that doesn't exist on !art)

you also need `core-libart-host.jar` for some stuff, seems to work as-is with the one compiled
against/as part of art bootstrap in dalvik-standalone.

some stuff in libcore needs JNI counterparts in libjavacore.so,
make yourself a patched one in `libcore_natives_nonart` (without patches it doesn't load properly)
(this is not needed for endless-tunnel)

for Gravity Defied, you will need `apachehttp-host.jar` (or patch out the "stat" tracking),
specifically you'll want it to be compiled against non-art bootstrap (though for *launching*
Gravity Defied, the one built in dalvik-standalone works)

###### native image

in `native_image_stuff`, there are:

`api-impl.jar`, `ARSCLib.jar`, `core-libart-host.jar`  
how these were acquired is explained above

`liblooper.so` includes looper stuff extracted from art repo (linked as follows)  
`gcc -shared -o liblooper.so Looper.o RefBase.o VectorImpl.o SharedBuffer.o`

`liblog.so` is taken as-is from art build

`metadata` obtained from running endless-tunnel, making sure that everything necessary to launch endless-tunnel gets compiled

`data` contains stuff extracted from endless-tunnel apk, so that the apk isn't needed

`dist` contains the "finished product" - endless tunnel "ported" to Linux, with no java needed except the 16MB monstrosity that the java stuff was compiled to
(kinda sad that the dead code elimination decided we still need 4MB worth of java stdlib)

to create libatl-java (which you will link the main executable against in place of libart/libjvm):  
`/path/to/graalvm-jdk-21.0.1+12.1/bin/native-image --shared -H:JNIConfigurationFiles=./metadata/jni-config.json -H:ReflectionConfigurationFiles=./metadata/reflect-config.json -H:Name=libatl-java -cp 'core-libart-host.jar:apachehttp-host.jar:ARSCLib.jar:api-impl.jar'`

to run endless tunnel "linux port": put everything together and run `LINKER_DIE_AT_RUNTIME= LD_PRELOAD=liblooper.so LD_LIBRARY_PATH=. ./android-translation-layer`

NOTE: endless tunnel contains no java code (it uses the AOSP/atl-provided NativeActivity class for it's Activity), any app which contains java code would need it's classes provided when building the native-image.

Experiment conclusions: dead code elimination doesn't seem to be able to reduce the amount of java code as much as I'd hoped, and it's unclear how much this can be optimized by adding checks for native image build
(which would take code paths that make more sense for a "port" as opposed the full atl), since the largest contributors to the library's size are java.base and some String stuff (the 2MB for reflection could be eliminated)

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

NOTE: some apps don't like runtime changes to resolution, and currently GLSurfaceView will stretch instead of changing resolution  
to sidestep this, we allow for specifying the initial resolution, which will currently always get passed as the screen resolution to GLSurfaceView even when you resize the window.
example with custom width/height: `android-translation-layer path/to/org.happysanta.gd_29.apk -l org/happysanta/gd/GDActivity -w 540 -h 960`

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
you can look at simple widgets (e.g. TextView, or ImageView) to see how to implement a widget such that  
it shows up as a Gtk Widget.  

for more specific instructions, see `doc/QuickHelp.md`
for general description of the architecure, see `doc/Architecture.md`

screenshot:

![angry birds 3.2.0, Worms 2 Armageddon, and gravity defied running side by side by side](https://gitlab.com/Mis012/android_translation_layer_PoC/-/raw/master/screenshot_2.png)

##### Roadmap:

- fix issues mentioned above

- fix ugly hacks
	- figure out a way to dynamically change the resolution of a GLSurfaceView (probably just implement it in terms of SurfaceView, though that one currently doesn't work on X11)

- implement more stuff (there is a lot of it, and it won't get done if nobody helps... ideally pick a simple-ish application and stub/implement stuff until it works)

- explore using bubblewrap to enforce the security policies that google helpfully forces apps to comply with (and our own security policies, like no internet access for apps which really shouldn't need it and are not scummy enough to refuse to launch without it)

##### Tips:

- the correct format for changing verbosity of messages going through android's logging library is `ANDROID_LOG_TAGS=*:v` (where `*` is "all tags" and `v` is "verbosity `verbose` or lesser"  
(note that specifying anything other than `*` as the tag will not work with the host version of liblog)
