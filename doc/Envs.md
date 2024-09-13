The following environment variables are recognized by the main executable:

`JDWP_LISTEN=<port>` - if set, art will listen for a jdb connection at `<port>`  
`RUN_FROM_BUILDDIR=` - if set, will search for `api-impl.jar` and `libtranslation_layer.so` in current working directory (may need `LD_LIBRARY_PATH=.` as well for `libandroid.so.0`)  
`ANDROID_APP_DATA_DIR=<path>` - if set, overrides the default path of `~/.local/android_translation_layer` for storing app data  
`ATL_DISABLE_WINDOW_DECORATIONS=` - if set, window decorations will be disabled; 
this is useful for saving screen space on phone screens, as well as working around the fact that we currently don't account for the titlebar when passing screen size to apps  
`ATL_UGLY_ENABLE_LOCATION=` - if set, apps will be able to get location data using the relevant android APIs. (TODO: use bubblewrap)  
`ATL_UGLY_ENABLE_WEBVIEW=` - if not set, WebView will be stubbed as a generic View; this will avoid wasting resources on WebViews which are only used for fingerprinting and ads  
`ATL_FORCE_FULLSCREEN` - if set, will fullscreen the app window on start; this is useful for saving screen space on phone screens, as well as making apps that can't handle arbitrary screen dimensions for some reason happier  
`ATL_SKIP_NATIVES_EXTRACTION` - if set, natives will not be extracted automatically; it's already possible to replace a native lib, but removing it entirely will normally result in it getting re-extracted, which may not be what you want
