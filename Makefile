CURRDIR := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))

all: compile_and_covert_hax compile_and_covert_launcher compile_jni

clean:
	find main-src -name *.class -exec rm {} + || true
	find xmlpull -name *.class -exec rm {} + || true
	rm hax.dex hax_xmlpull.dex main.dex || true
	rm libnative/org_launch_main.so || true

default: all

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

compile_hax: | convert_xmlpull
	javac -h jni -source 1.7 -target 1.7 -cp \
	$(CURRDIR)/jars/core.jar:\
	$(CURRDIR)/xmlpull/ \
	main-src/android/content/Context.java \
	main-src/android/os/Bundle.java \
	main-src/android/app/Activity.java \
	main-src/android/support/v4/app/FragmentActivity.java \
	main-src/android/view/View.java \
	main-src/android/view/ViewGroup.java \
	main-src/android/view/LinearLayout.java \
	main-src/android/widget/TextView.java \
	main-src/android/util/AttributeSet.java \
	main-src/android/util/Xml.java \
	main-src/android/util/XmlPullAttributes.java \
	main-src/com/android/internal/util/XmlUtils.java \
	main-src/com/android/internal/util/FastXmlSerializer.java \
	main-src/android/view/LayoutInflater.java

convert_hax: | compile_hax
	cd main-src/ && ~/Android/Sdk/build-tools/21.0.0/dx --verbose --dex --output=../hax.dex \
	android/content/*.class \
	android/app/*.class \
	android/support/v4/app/*.class \
	android/widget/*.class \
	android/view/*.class \
	android/os/*.class \
	android/util/*.class \
	com/android/internal/util/*.class

convert_xmlpull: | compile_xmlpull
	cd xmlpull/ && ~/Android/Sdk/build-tools/21.0.0/dx --verbose --dex --output=../hax_xmlpull.dex \
	org/kxml2/io/KXmlSerializer.class \
	org/kxml2/io/KXmlParser.class \
	org/xmlpull/v1/XmlPullParser.class \
	org/xmlpull/v1/sax2/Driver.class \
	org/xmlpull/v1/XmlPullParserException.class \
	org/xmlpull/v1/XmlPullParserFactory.class \
	org/xmlpull/v1/XmlSerializer.class

compile_and_covert_hax: compile_xmlpull convert_xmlpull compile_hax convert_hax

compile_launcher:
	javac -h jni -source 1.6 -target 1.6 -cp \
	$(CURRDIR)/demo_app.jar:\
	$(CURRDIR)/main-src/:\
	$(CURRDIR)/xmlpull/ \
	main-src/org/launch/main.java 

convert_launcher: | compile_launcher
	cd main-src/ && ~/Android/Sdk/build-tools/21.0.0/dx --verbose --dex --output=../main.dex org/launch/*.class

compile_and_covert_launcher: compile_launcher convert_launcher

compile_jni: | compile_hax
	mv jni/android_view_LinearLayout.h jni/android_view_ViewGroup.h jni/views/
	mv jni/android_widget_TextView.h jni/widgets
	gcc -g -shared -fPIC -o libnative/org_launch_main.so -I /usr/lib64/jvm/java/include/ -I /usr/lib64/jvm/java/include/linux/ -m32 -fPIE `PKG_CONFIG_PATH=/usr/lib/pkgconfig/ pkgconf gtk4 --cflags --libs` jni/*.c jni/widgets/*.c jni/views/*.c

run:
	./dalvik/dalvik -verbose:jni -cp hax_xmlpull.dex:hax.dex:main.dex:demo_app.apk org/launch/main com/example/demo_application/MainActivity
