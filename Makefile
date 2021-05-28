CURRDIR := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))

compile_hax:
	javac -h jni -source 1.7 -target 1.7 -cp \
	$(CURRDIR)/jars/core.jar \
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
	xmlpull/org/kxml2/io/KXmlSerializer.java \
	xmlpull/org/kxml2/io/KXmlParser.java \
	xmlpull/org/xmlpull/v1/XmlPullParser.java \
	xmlpull/org/xmlpull/v1/sax2/Driver.java \
	xmlpull/org/xmlpull/v1/XmlPullParserException.java \
	xmlpull/org/xmlpull/v1/XmlPullParserFactory.java \
	xmlpull/org/xmlpull/v1/XmlSerializer.java \
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

convert_hax_xmlpull:
	cd xmlpull/ && ~/Android/Sdk/build-tools/21.0.0/dx --verbose --dex --output=../hax_xmlpull.dex \
	org/kxml2/io/KXmlSerializer.class \
	org/kxml2/io/KXmlParser.class \
	org/xmlpull/v1/XmlPullParser.class \
	org/xmlpull/v1/sax2/Driver.class \
	org/xmlpull/v1/XmlPullParserException.class \
	org/xmlpull/v1/XmlPullParserFactory.class \
	org/xmlpull/v1/XmlSerializer.class

compile_and_covert_hax: compile_hax convert_hax

compile_launcher:
	javac -h jni -source 1.6 -target 1.6 -cp \
	demo_app.jar:\
	main-src/ \
	main-src/org/launch/main.java 

convert_launcher: | compile_launcher
	cd main-src/ && ~/Android/Sdk/build-tools/21.0.0/dx --verbose --dex --output=../main.dex org/launch/*.class

compile_and_covert_launcher: compile_launcher convert_launcher

compile_jni:
	gcc -g -shared -fPIC -o libnative/org_launch_main.so -I /usr/lib64/jvm/java/include/ -I /usr/lib64/jvm/java/include/linux/ -m32 -fPIE `PKG_CONFIG_PATH=/usr/lib/pkgconfig/ pkgconf gtk4 --cflags --libs` jni/org_launch_main.c

run:
	./dalvik/dalvik -verbose:jni -cp hax_xmlpull.dex:hax.dex:main.dex:demo_app.apk org/launch/main com/example/demo_application/MainActivity
