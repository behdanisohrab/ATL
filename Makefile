CURRDIR := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))

compile_launcher:
	javac -source 1.6 -target 1.6 -cp \
	BasicImmersiveMode_before_dex.jar:\
	$(CURRDIR)/jars/android.jar:\
	$(CURRDIR)/jars/uiautomator.jar:\
	$(CURRDIR)/jars/layoutlib.jar:\
	$(CURRDIR)/jars/icu4j.jar:\
	$(CURRDIR)/jars/core.jar:\
	$(CURRDIR)/jars/framework.jar\
	$(CURRDIR)/jars/support-v4-21.0.0-sources.jar:\
	$(CURRDIR)/jars/cardview-v7-21.0.0-sources.jar:\
	$(CURRDIR)/jars/leanback-v17-21.0.0-sources.jar:\
	$(CURRDIR)/jars/palette-v7-21.0.0-sources.jar:\
	$(CURRDIR)/jars/recyclerview-v7-21.0.0-sources.jar:\
	$(CURRDIR)/jars/support-annotations-21.0.0-sources.jar:\
	$(CURRDIR)/jars/support-v13-21.0.0-sources.jar:\
	$(CURRDIR)/jars/support-v4-21.0.0-sources.jar:\
	$(CURRDIR)/jars/support-v4-19.1.0.jar \
	main-src/org/launch/main.java

convert_launcher:
	cd main-src/ && ~/Android/Sdk/build-tools/21.0.0/dx --verbose --dex --output=../main.dex org/launch/*.class

run:
	./dalvik/dalvik -cp BasicImmeriveMode.apk:main.dex org/launch/main
