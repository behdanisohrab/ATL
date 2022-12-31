##### known working cmdline for enless tunnel:  
`SLES_SDL_FREQ=8000 AUDIODEV=sysdefault:CARD=Generic_1 SDL_AUDIODRIVER=alsa LINKER_DIE_AT_RUNTIME= RUN_FROM_BUILDDIR= LD_LIBRARY_PATH=./:$PATH_TO_LIBOPENSLES_LOCATION_IF_NOT_INSTALLED_SYSTEM_WIDE ./android-translation-layer ../test_apks/ndk_sample_endless_tunnel.apk -l android/app/NativeActivity -w 700 -h 700`
explanation:  
`AUDIODEV=sysdefault:CARD=Generic_1 SDL_AUDIODRIVER=alsa` - SDL doesn't like the defaults, ymmv  
`SLES_SDL_FREQ=8000` - the app uses a sample rate of 8000, and the libOpenSLES SDL backend only supports a single sample rate set at the library load time  
`LINKER_DIE_AT_RUNTIME=` - the app can optionally use joystick input, and there are functions for that which we don't currently have stubbed; however since they never get called...  
`$PATH_TO_LIBOPENSLES_LOCATION_IF_NOT_INSTALLED_SYSTEM_WIDE` - compile `https://gitlab.com/Mis012/libopensles-standalone/` and either install it system wide or provide path to it in `LD_LIBRARY_PATH`  

##### known working cmdline for OpenXR sample:
`DRI_PRIME=1 UGLY_HACK_FOR_VR= RUN_FROM_BUILDDIR= ./android-translation-layer ../test_apks/oculus_vr_controller_input.apk -l android/app/NativeActivity`
explanation:
`DRI_PRIME=1` - iGPU was behaving sus, ymmv
`UGLY_HACK_FOR_VR=` - Wayland doesn't support pbuffers, so we let the app use XWayland (which is the default) instead of overriding the EGLDisplay as we normally would in order to enable drawing on our Wayland surface
additional notes:
`OXR_DEBUG_GUI=1 monado-service` and use of the "debug readout of left eye" seem to be needed when not using a real HMD, ymmv

##### known working cmdline for unity sample:
`MESA_GL_VERSION_OVERRIDE=3.2 MESA_GLSL_VERSION_OVERRIDE=320 LINKER_DIE_AT_RUNTIME= RUN_FROM_BUILDDIR= LD_LIBRARY_PATH=./ ./android-translation-layer ../test_apks/sample_unity_app.apk -l com/unity3d/player/UnityPlayerActivity`
explanation:  
`MESA_GL_VERSION_OVERRIDE=3.2 MESA_GLSL_VERSION_OVERRIDE=320` - Unity seems to do a very dumb thing,
where it checks what the available non-es GLSL version is (so 4xx on many devices) and then appends `es`
to it (on some devices creating 4xx es - something that literally doesn't exist); if we tell it that
we only have OpenGL 3.2, it will proceed to try and use OpenGL 3.2 ES, which actually exists
`LINKER_DIE_AT_RUNTIME=` - similar to above, functions are linked against which are not *really* needed
