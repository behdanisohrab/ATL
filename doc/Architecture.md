#### directory structure

`dalvik/*dalvik` - helper scripts for running bytecode in the dalvik VM (not used anymore, we link against `libdvm.so` directly now)
`dalvik/linux-${arch}/` - pre-complied dalvik from https://gitlab.com/Mis012/dalvik_standalone

`arsc_parser/` - Java .arsc parser I found somewhere, with fixes (should eventually get replaced by C code)
`data/` - the equivalent of `/data/data/${app-package-name}/`; TODO - use `data/${app-package-name}` instead 
to allow for storing the data of multiple apps at the same time
`data/lib/` - hardcoded location, libraries under which are assumed to be linked against bionic 
(and will therefore be loaded with a shim bionic linker)
`doc/` - documentation
`jars/` - when we want to link against dalvik core java libs, we need to keep their non-dex versions here
`jni/` - C code implementing things which it doesn't make sense to do in Java (ideally this would be most things)
`libandroid-src/` - C code implementing `libandroid.so` (this is needed by most JNI libs which come with android apps)
`libnative/` - compilation output for our `.so` libraries (currently `libtranslation_layer_main.so` and `libandroid.so`)
`main-src/` - Java code implementing the android APIs
`src/` - code for the main executable, which sets stuff up (including the JVM) and launches the activity specified on the cmdline
`test-apks/` - all apks known to somewhat work, except paid proprietary ones which we sadly can't really include

`./com.google.android.gms.apk`: microg; stopgap solution to run apps with GSF dependencies (fwiw, the actual solution will likely also use MicroG)
`./*.dex` - compiled Java code; `hax_arsc_parser.dex` corresponds to `hax_arsc_parser.dex/`,
`hax.dex` corresponds to `main-src`

#### philosophy

companion infographic for this section:
`doc/android_translation_architecture.svg`

We believe that the cleanest approach for supporting android apps on desktop
Linux platforms is to make a chirurgical cut on the android platform stack
as close to the apps themselves as possible, and sew on a new implementation
of whatever we have cut off.

If you glance at the companion infographic, you can see that the place where
we chose to make the cut is directly between the Apps and the Java APIs
provided by the android frameworks. (additionally, for apps with native
components, we also keep just the respective .so files from those apps
and provide implementations/shims for the system libraries they are linked
against)

#### current control flow (to be refined)

1. the user executes `launch_activity.sh`  
this wrapper accomplishes the following things:
	- `LD_PRELOAD` makes sure that `libbsd` (which provides some functions that bionic would on android) is loaded, even though bionic-linked libraries don't know they want it
	- `LD_PRELOAD` also loads libpthread_bio.so, which is a shim for translating libpthread calls made by bionic-linked libraries to calls to glibc/musl libpthread
	- `ANDROID_ROOT` and `ANDROID_DATA` get set, both of which are required by dalvik
	- `BIONIC_LD_LIBRARY_PATH` gets set to the path to the android app's lib folder (currently any app's data directory is `data/`) so that the shim bionic linker knows which .so files must not be loaded by the system linker
	- `LD_LIBRARY_PATH` is set up such that both the host linker and the shim bionic linker can find any non-system libraries they will need
	- `./main ${1} -l ${2}` calls the main executable and instructs it to load the apk file `${1}` into classpath before launching the activity `${2}` using the JVM

2. the executable is compiled from `src/main.c`:
	1. `int main(int argc, char **argv)` sets up a GtkApllication, cmdline handling, and calls `g_application_run`
	2. GtkApplication glue parses the cmdline and calls `static void open(GtkApplication *app, GFile** files, gint nfiles, const gchar* hint, struct jni_callback_data *d)`
	3. `static void open(GtkApplication *app, GFile** files, gint nfiles, const gchar* hint, struct jni_callback_data *d)`:
		1. constructs the classpath from the following:
			- `hax_arsc_parser.dex` is dalvik bytecode implementing .arsc parsing duties (to be replaced by C code eventually)
			- `hax.dex` contains all the implementations of android framework functions
			- `%s` will be substituted by the path to the app's apk (passed to us on cmdline), making the bytecode within (and resources.arsc, which is currently the only other file read straight from the apk) available in classpath
			- `com.google.android.gms.apk` is the path to a microG apk, needed for apps with a dependency on GSF; this is specified after the app's apk so that the the app's apk is the first zip file in the classpath (needed for getting the right resources.arsc, TODO: ask for the classloader which loaded the activity that was specified on the cmdline)
		2. contructs other options for and launches the dalvik virtual machine
		3. load `libtrasnlation_layer_main.so` using the internal function `dvmLoadNativeCode` (this contains our native methods, and dalvik will only search in libraries that were loaded using this or System.load) (NOTE: care is taken to register this with the right classloader so the java code which declares the native methods is the java code that can use them)
		4. sets up a JNI handle cache
		5. sets display size to be passed to apps according to optionally specified window size (some apps handle runtime resizing poorly, and our EGL code currently doesn't handle it at all)
		6. creates a Gtk Window with said size and shows it on screen
		7. calls the OnCreate method of the class specified with the `-l` option

3. the Activity specfified with `-l` calls various android APIs, which we hopefully implement; 
typically, it will set up some android widgets, which will appear in the Gtk window as Gtk widgets

5. you, the user, interact with the Gtk widgets, which the app might have registered callbacks for; 
as long as our implementation of android APIs is good enough for what the app needs, you can use the app
as you would on android
