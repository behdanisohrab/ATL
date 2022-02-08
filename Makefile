CURRDIR := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))

all: compile_and_covert_hax compile_and_covert_launcher compile_jni compile_libandroid

clean:
	find main-src -name *.class -exec rm {} + || true
	find xmlpull -name *.class -exec rm {} + || true
	rm hax.dex hax_xmlpull.dex main.dex || true
	rm libnative/org_launch_main.so || true

default: all

compile_arsc_parser:
	javac -source 1.7 -target 1.7 -bootclasspath \
	$(CURRDIR)/jars/core.jar \
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

compile_xmlpull:
	javac -h jni -source 1.7 -target 1.7 -cp \
	$(CURRDIR)/jars/core.jar \
	xmlpull/org/kxml2/io/KXmlSerializer.java \
	xmlpull/org/kxml2/io/KXmlParser.java \
	xmlpull/org/xmlpull/v1/XmlPullParser.java \
	xmlpull/org/xmlpull/v1/sax2/Driver.java \
	xmlpull/org/xmlpull/v1/XmlPullParserException.java \
	xmlpull/org/xmlpull/v1/XmlPullParserFactory.java \
	xmlpull/org/xmlpull/v1/XmlSerializer.java

convert_xmlpull: | compile_xmlpull
	cd xmlpull/ && $(CURRDIR)/dalvik/linux-x86/bin/dx --verbose --dex --output=../hax_xmlpull.dex \
	org/kxml2/io/KXmlSerializer.class \
	org/kxml2/io/KXmlParser.class \
	org/xmlpull/v1/XmlPullParser.class \
	org/xmlpull/v1/sax2/Driver.class \
	org/xmlpull/v1/XmlPullParserException.class \
	org/xmlpull/v1/XmlPullParserFactory.class \
	org/xmlpull/v1/XmlSerializer.class

compile_hax: | convert_xmlpull
	javac -h jni -source 1.7 -target 1.7 -bootclasspath \
	$(CURRDIR)/jars/core.jar \
	-cp \
	$(CURRDIR)/arsc_parser/:\
	$(CURRDIR)/xmlpull/ \
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
	com/google/android/vending/licensing/*.class \
	javax/microedition/khronos/egl/*.class \
	javax/microedition/khronos/opengles/*.class \
	com/google/android/gles_jni/*.class \
	android/*.class \
	com/android/internal/*.class

compile_and_covert_hax: compile_arsc_parser convert_arsc_parser compile_xmlpull convert_xmlpull compile_hax convert_hax

compile_launcher:
	javac -h jni -source 1.6 -target 1.6 -bootclasspath \
	$(CURRDIR)/jars/core.jar \
	-cp \
	$(CURRDIR)/main-src/:\
	$(CURRDIR)/xmlpull/ \
	main-src/org/launch/main.java 

convert_launcher: | compile_launcher
	cd main-src/ && $(CURRDIR)/dalvik/linux-x86/bin/dx --verbose --dex --output=../main.dex org/launch/*.class

compile_and_covert_launcher: compile_launcher convert_launcher

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
	gcc -g -m32 -shared -fPIC -o libnative/org_launch_main.so -I /usr/lib64/jvm/java/include/ -I /usr/lib64/jvm/java/include/linux/ jni/*.c jni/widgets/*.c jni/views/*.c jni/drawables/*.c jni/egl/*.c `PKG_CONFIG_PATH=/usr/lib/pkgconfig/ pkgconf gtk4 --cflags --libs`

compile_libandroid:
	gcc -m32 -shared -fPIC -o libnative/libandroid.so libandroid-src/*.c
