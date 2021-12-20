### PoC for a translation layer that would execute the abolute minimum of android-specific code necessary in order to run an android app on a Linux system

---

instructions:  
`make`: compile everything (except dalvik, which is precompiled from https://gitlab.com/Mis012/dalvik_standalone)  
`make run`: launch the demo app using the translation layer  

screenshot:

![gravity defied, simple demo app, and another gravity defied running side by side by side](https://gitlab.com/Mis012/android_translation_layer_PoC/-/raw/master/screenshot.png)

note: running two different apps at the same time is a bit of a hack, since the data directory path is currently hardcoded to `./data/` no matter the app

##### FAQ:

Q:  
	did you ever notice that most apps nowadays use at least some .so libraries?  
	some apps even use them for 99% of their functionality!  

A:  
	yep, and that *can* be tackled, see https://github.com/minecraft-linux/mcpelauncher-linker or https://github.com/Cloudef/android2gnulinux/

Q:  
	<del>hey! this is so basic it's useless!</del>  
A:  
	yup, we still need to do some proper reimplementing :)  
	<del>The PoC works-ish now, though it's much less complex than even a modern sample app.  
	For one, it doesn't use any of the complex compat layers, it just directly subclasses Activity.  
	Second, it uses only two basic UI elements (TextView and LinearLayout), and yes these are  
	the only ones implemented (and partially at that, only the absolutely required functionality).  
	Third, it doesn't do much - it just sets up it's static, non-changing UI (though it can process  
	a layout xml, as well as reflect changes to a TextView done in OnCreate)</del>

Q:  
	ok, so this can sorta run Gravity Defied. What's the catch?  

A:  
	well, first things first, technically I compiled Gravity Defied myself and removed  
	some bug-reporting-init-related code which I got frustrated with stubbing out. however,  
	adding more stubs should make that unnecessary.  
	now for the second issue: `./data/` contains some stuff that should instead be read from the apk,  
	and some of this stuff is also externally converted (e.g Xml from binary form to actual Xml).  
	obviously this is not ideal for user experience. NOTE: it seems that the binary form *might* be  
	protobuf-based, which would make reading it directly easier.  
	and the third issue: Gravity Defied is still extremely simple compared to most android apps,  
	doesn't acknowledge compat layers, and the most intricate UI element is completely custom drawn  
	using the canvas interface, in a manner that makes it easy to implement with cairo.

##### Roadmap:

- fix issues mention above

- fix ugly hacks

- implement more stuff (there is a lot of it, and it won't get done if nobody helps... ideally pick a simple-ish application and stub/implement stuff until it works)

- integrate e.g. https://github.com/Cloudef/android2gnulinux/ for loading libraries linked against bionic; if we're lucky, 
we might be able to get quite a few apps running that are 99% native, since they hopefully only use a few Java APIs and might not even complain too much if most of those APIs are stubbed
