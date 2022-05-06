#### directory structure

`dalvik/*dalvik` - helper scripts for running bytecode in the dalvik VM
`dalvik/linux-${arch}` - pre-complied dalvik from https://gitlab.com/Mis012/dalvik_standalone

`arsc_parser/` - Java .arsc parser I found somewhere, with fixes (should eventually get replaced by C code)
`data` - the equivalent of `/data/data/${app-package-name}/`; TODO - use `data/${app-package-name}` instead 
to allow for storing the data of multiple apps at the same time
`data/lib/` - hardcoded location, libraries under which are assumed to be linked against bionic 
(and will therefore be loaded with a shim bionic linker)
`doc` - documentation
`jars` - when we want to link against dalvik core java libs, we need to keep their non-dex versions here
`jni` - C code implementing things which it doesn't make sense to do in Java (ideally this would be most things)
`libandroid-src` - C code implementing `libandroid.so` (this is needed by most JNI libs which come with android apps)
`libnative` - compilation output for `.so` libraries
`main-src/org/launch/main.java` - the entry point; TODO - at this point it just calls into the C entry point, 
so should probably move the entry point to C
`main-src/` (rest) - Java code implementing the android APIs
`test-apks` - all apks known to somewhat work, except paid proprietary ones

`./com.google.android.gms.apk`: microg; stopgap solution to run apps with GSF dependencies
`./*.dex` - compiled Java code; `hax_arsc_parser.dex` corresponds to `hax_arsc_parser.dex/`, `main.dex` 
corresponds to `main-src/org/launch/main.java` and `hax.dex` corresponds to the rest of `main-src`

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
(contents:  
```sh
LD_PRELOAD=libpthread_bio.so:libbsd.so.0 ./dalvik/dalvik -verbose:jni -cp hax_arsc_parser.dex:hax.dex:main.dex:${1}:com.google.android.gms.apk org/launch/main -l ${2}
```
)  
this line accomplishes the following things:
	- `LD_PRELOAD` makes sure that `libbsd` (which provides some functions that bionic would on android) is loaded, even though bionic-linked libraries don't know they want it
	- `LD_PRELOAD` also loads libpthread_bio.so, which is a shim for translating libpthread calls made by bionic-linked libraries to calls to glibc/musl libpthread
	- `./dalvik/dalvik` is a helper script for launching the dalvik vm with the right `LD_LIBRARY_PATH`, `bootclasspath`, etc
	- `hax_arsc_parser.dex` is dalvik bytecode implementing .arsc parsing duties (to be replaced by C code eventually)
	- `hax.dex` contains all the implementations of android framework functions
	- `main.dex` contains the Java entry point `public static void main(String[])`
	- `${1}` will be substituted by the path to the app's apk, making the bytecode within (and resources.arsc, which is currently the only other file read straight from the apk) available in classpath
	- `com.google.android.gms.apk` is the path to a microG apk, needed for apps with a dependency on GSF; this is specified after the app's apk so that the the app's apk is the first zip file in the classpath (needed for getting the right resources.arsc)
	- `org/launch/main` is the class which contains the Java entry point, this tells dalvik to look for it there
	- `-l ${2}` is cmdline argument list to be passed to the entry point function, which passes it to the C entry point

2. the Java entry point is executed by dalvik  
(contents:  
```Java
public class main {
	public static void main(String[] args) {
		System.load("libnative/org_launch_main.so");
		real_main(args);
	}

	public static native void real_main(String[] args);
}
M
```
);  

3. execution jumps to the C entry point in `jni/org_launch_main.c`
	1. the function `JNIEXPORT void JNICALL Java_org_launch_main_real_1main(JNIEnv *env, jclass this_class, jobjectArray args)` fixes up argc/argv in preparation for removing the Java entry point in favor of a  C entry point, and calls `static int main(int argc, char **argv, JNIEnv *env)`
	2. `static int main(int argc, char **argv, JNIEnv *env)` sets up a GtkApllication, cmdline handling, and calls `g_application_run`
	3. GtkApplication glue parses the cmdline and calls `static void activate(GtkApplication *app, struct jni_callback_data *d)`
	4. `static void activate(GtkApplication *app, struct jni_callback_data *d)`:
		1. sets up a JNI handle cache
		2. sets display size to be passed to apps according to optionally specified window size
		3. creates a Gtk Window with said size and shows it on screen
		4. calls the OnCreate method of the class specified with the `-l` option

4. the Activity specfified with `-l` calls various android APIs, which we hopefully implement; 
typically, it will set up some android widgets, which will appear in the Gtk windows as Gtk widgets

5. you, the user, interact with the Gtk widgets, which the app might have registered callbacks for; 
as long as our implementation of android APIs is good enough for what the app needs, you can use the app
as you would on android
