known working cmdline for enless tunnel:  
`SLES_SDL_FREQ=8000 AUDIODEV=sysdefault:CARD=Generic_1 SDL_AUDIODRIVER=alsa LINKER_DIE_AT_RUNTIME= RUN_FROM_BUILDDIR= LD_LIBRARY_PATH=./:$PATH_TO_LIBOPENSLES_LOCATION_IF_NOT_INSTALLED_SYSTEM_WIDE ./android-translation-layer ../test_apks/ndk_sample_endless_tunnel.apk -l android/app/NativeActivity -w 700 -h 700`
explanation:  
`AUDIODEV=sysdefault:CARD=Generic_1 SDL_AUDIODRIVER=alsa` - SDL doesn't like the defaults, ymmv  
`SLES_SDL_FREQ=8000` - the app uses a sample rate of 8000, and the libOpenSLES SDL backend only supports a single sample rate set at the library load time  
`LINKER_DIE_AT_RUNTIME=` - the app can optionally use joystick input, and there are functions for that which we don't currently have stubbed; however since they never get called...  
`$PATH_TO_LIBOPENSLES_LOCATION_IF_NOT_INSTALLED_SYSTEM_WIDE` - compile `https://gitlab.com/Mis012/libopensles-standalone/` and either install it system wide or provide path to it in `LD_LIBRARY_PATH`  
