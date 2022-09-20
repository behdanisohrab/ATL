CURRDIR := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))

all: compile_and_covert_hax compile_jni compile_libandroid

clean:
	find main-src -name *.class -exec rm {} + || true
	rm hax_arsc_parser.dex hax.dex || true
	rm libnative/libtranslation_layer_main.so || true

default: all

compile_arsc_parser:
	javac -source 1.7 -target 1.7 -bootclasspath \
	$(CURRDIR)/jars/core-libart-hostdex_classes.jar \
	arsc_parser/com/hq/arscresourcesparser/*.java \
	arsc_parser/com/hq/arscresourcesparser/arsc/*.java \
	arsc_parser/com/hq/arscresourcesparser/common/*.java \
	arsc_parser/com/hq/arscresourcesparser/stream/*.java

convert_arsc_parser: | compile_arsc_parser
	cd arsc_parser/ && $(CURRDIR)/dalvik/linux-x86/bin/dx --verbose --dex --output=../hax_arsc_parser.dex \
	com/hq/arscresourcesparser/*.class \
	com/hq/arscresourcesparser/arsc/*.class \
	com/hq/arscresourcesparser/common/*.class \
	com/hq/arscresourcesparser/stream/*.class

compile_hax: | compile_arsc_parser
	javac -h jni -source 1.7 -target 1.7 -bootclasspath \
	$(CURRDIR)/jars/core-libart-hostdex_classes.jar \
	-cp \
	$(CURRDIR)/arsc_parser/ \
	main-src/android/content/*.java \
	main-src/android/os/*.java \
	main-src/android/app/*.java \
	main-src/android/support/v4/app/FragmentActivity.java \
	main-src/android/view/*.java \
	main-src/android/widget/*.java \
	main-src/android/text/*.java \
	main-src/android/text/util/*.java \
	main-src/android/util/*.java \
	main-src/com/android/internal/util/*.java \
	main-src/android/annotation/*.java \
	main-src/android/content/pm/*.java \
	main-src/android/content/res/*.java \
	main-src/android/graphics/*.java \
	main-src/android/database/*.java \
	main-src/android/database/sqlite/*.java \
	main-src/android/opengl/*.java \
	main-src/android/hardware/*.java \
	main-src/android/provider/*.java \
	main-src/android/net/*.java \
	main-src/android/telephony/*.java \
	main-src/android/location/*.java \
	main-src/android/media/*.java \
	main-src/android/webkit/*.java \
	main-src/android/preference/*.java \
	main-src/com/google/android/vending/licensing/*.java \
	main-src/javax/microedition/khronos/egl/*.java \
	main-src/javax/microedition/khronos/opengles/*.java \
	main-src/com/google/android/gles_jni/*.java \
	main-src/android/Manifest.java \
	main-src/android/R.java \
	main-src/com/android/internal/Manifest.java \
	main-src/com/android/internal/R.java

convert_hax: | compile_hax
	cd main-src/ && $(CURRDIR)/dalvik/linux-x86/bin/dx --verbose --dex --output=../hax.dex \
	android/content/*.class \
	android/app/*.class \
	android/support/v4/app/*.class \
	android/widget/*.class \
	android/view/*.class \
	android/text/*.class \
	android/text/util/*.class \
	android/os/*.class \
	android/util/*.class \
	com/android/internal/util/*.class \
	android/annotation/*.class \
	android/content/pm/*.class \
	android/content/res/*.class \
	android/graphics/*.class \
	android/database/*.class \
	android/database/sqlite/*.class \
	android/opengl/*.class \
	android/hardware/*.class \
	android/provider/*.class \
	android/net/*.class \
	android/telephony/*.class \
	android/location/*.class \
	android/media/*.class \
	android/webkit/*.class \
	android/preference/*.class \
	com/google/android/vending/licensing/*.class \
	javax/microedition/khronos/egl/*.class \
	javax/microedition/khronos/opengles/*.class \
	com/google/android/gles_jni/*.class \
	android/*.class \
	com/android/internal/*.class

compile_and_covert_hax: compile_arsc_parser convert_arsc_parser compile_hax convert_hax

compile_jni: | compile_hax
	mv jni/android_view_View.h \
	jni/android_view_ViewGroup.h \
	jni/views/
	mv jni/android_widget_TextView.h \
	jni/android_widget_ScrollView.h \
	jni/android_widget_RelativeLayout.h \
	jni/android_widget_LinearLayout.h \
	jni/android_widget_FrameLayout.h \
	jni/android_widget_ImageView.h \
	jni/android_opengl_GLSurfaceView.h \
	jni/widgets
	mv jni/com_google_android_gles_jni_EGLImpl.h \
	jni/egl/
	mv jni/android_media_AudioTrack.h \
	jni/audio/
	gcc -g -shared -fPIC -lasound -o libnative/libtranslation_layer_main.so \
	-I /usr/lib64/jvm/java/include/ -I /usr/lib64/jvm/java/include/linux/ \
	jni/*.c \
	jni/widgets/*.c \
	jni/views/*.c \
	jni/drawables/*.c \
	jni/egl/*.c \
	jni/audio/*.c \
	`pkgconf gtk4 --cflags --libs`
	LIBRARY_PATH=libnative/:dalvik/linux-x86/lib64/ gcc -g -fPIC -o main -I /usr/lib64/jvm/java/include/ -I /usr/lib64/jvm/java/include/linux/ src/main.c `pkgconf gtk4 --cflags --libs` -ltranslation_layer_main -lart -lnativebridge -ldl -ldl_bio

compile_libandroid:
	gcc -g -shared -fPIC -o libnative/libandroid.so libandroid-src/*.c
