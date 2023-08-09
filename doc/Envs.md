The following environment variables are recognized by the main executable:

`JDWP_LISTEN=<port>` - if set, art will listen for a jdb connection at `<port>`  
`RUN_FROM_BUILDDIR=` - if set, will search for `libtranslation_layer.so` in current working directory  
`ANDROID_APP_DATA_DIR=<path>` - if set, overrides the default path of `~/.local/android_translation_layer` for storing app data  
`ATL_DISABLE_WINDOW_DECORATIONS=` - if set, window decorations will be disabled; this is useful for saving screen space on phone screens, as well as working around issues with input tracking for GLSurfaceView  
`ATL_UGLY_ENABLE_LOCATION=` - if set, apps will be able to get location data using the relevant android APIs. (TODO: use bubblewrap)  
`UGLY_HACK_FOR_VR` - if set, EGL will use XWayland display on Wayland. This means the app won't be able to draw to it's window, but it will be able to use pbuffers which are not available on wayland.  
`HAX_AUDIOTRACK_PCM_DEVICE` - NOTE: android.media.AudioTrack is currently re-stubbed since the implementation was not ideal and caused crashes. We should probably use libOpenSLES so we only have one place
where we deal with native audio.  
