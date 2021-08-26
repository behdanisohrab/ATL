### PoC for a translation layer that would execute the abolute minimum of android-specific code necessary in order to run an android app on a Linux system

---

instructions: see Makefile

##### FAQ:

Q:  
	did you ever notice that most apps nowadays use at least some .so libraries?  
	some apps even use them for 99% of their functionality!  

A:  
	yep, and that *can* be tackled, see https://github.com/minecraft-linux/mcpelauncher-linker

Q:  
	hey! this is so basic it's useless!  
A:  
	yup, we still need to do some proper reimplementing :)  
	The PoC works-ish now, though it's much less complex than even a modern sample app.  
	For one, it doesn't use any of the complex compat layers, it just directly subclasses Activity.  
	Second, it uses only two basic UI elements (TextView and LinearLayout), and yes these are  
	the only ones implemented (and partially at that, only the absolutely required functionality).  
	Third, it doesn't do much - it just sets up it's static, non-changing UI (though it can process  
	a layout xml, as well as reflect changes to a TextView done in OnCreate)

##### Roadmap:

first we need to gather a lot of volunteers, because the android abi is *huge*  

then we need to settle on language / UI toolkit.  

I would like to shim stuff to C for performance, and use GTK 4 for UI - because  
that seems like a lot of fun, and we need to be honest with ourselves, why  
would anyone help with this project if not for fun ;)  

__UPDATE__:

- Started work on shimming stuff to C
- Use Gtk4 for displaying the app's GUI (see screenshot.png)
