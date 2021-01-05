### PoC for a translation layer that would execute the abolute minimum of android-specific code necessary in order to run an android app on a Linux system

---

instructions: see Makefile

##### FAQ:

Q:  
	did you ever notice that most apps nowadays use at least some .so libraries?  
	some apps even use them for 99% of their functionality!  

A:  
	yep, and that *can* be tackled, see https://github.com/minecraft-linux/{mcpelauncher-linker, ...}  

Q:  
	hey! it errors out!  
A:  
	yup, we need to do some reimplementing :)  
	for the PoC (simple sample app), just some basic stuff should be enough.  

	Google did a lot of work for us already, by making a tool that can stub  
	out all the source files. The stubbed out files are available in their Sdk,  
	but it would be smart to just use the tool on the much better licensed  
	original codebase.  

##### Roadmap:

first we need to gather a lot of volunteers, because the android abi is *huge*  

then we need to settle on language / UI toolkit.  

I would like to shim stuff to C for performance (kinda like Google already  
does with C++ on newer implementations), and use GTK 4 for UI - because  
that seems like a lot of fun, and we need to be honest with ourselves, why  
would anyone help with this project if not for fun ;)  
