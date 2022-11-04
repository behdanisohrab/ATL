project('android_translation_layer', ['c', 'java'], default_options: ['b_lundef=false'])

gnome = import('gnome')

incdir_dep = declare_dependency(include_directories: '.')
add_project_dependencies(incdir_dep, language: 'c')

cc = meson.get_compiler('c')
dir_base = meson.current_source_dir()
builddir_base = meson.current_build_dir()
# FIXME: make art install a pkgconfig file
libart_dep = [
	cc.find_library('art', dirs : [ '/usr' / get_option('libdir') / 'art', '/usr/local' / get_option('libdir') / 'art', get_option('prefix') / get_option('libdir') / 'art' ]),
     	cc.find_library('nativebridge', dirs : [ '/usr' / get_option('libdir') / 'art', '/usr/local' / get_option('libdir') / 'art', get_option('prefix') / get_option('libdir') / 'art' ])
]
libdl_bio_dep = [
	cc.find_library('dl_bio')
]

marshal_files = gnome.genmarshal('marshal',
	sources: 'src/api-impl-jni/widgets/marshal.list',
	valist_marshallers: true,
	internal: true,
)


# libandroid
libandroid_so = shared_library('android', [
                          	'src/libandroid/asset_manager.c',
                          	'src/libandroid/bitmap.c',
                          	'src/libandroid/configuration.c',
                          	'src/libandroid/input.c',
                          	'src/libandroid/looper.c',
                          	'src/libandroid/media.c',
                          	'src/libandroid/native_window.c',
                          	'src/libandroid/sensor.c',
                          ],
                          install: true,
                          soversion: 0,
                          dependencies: [
                          	dependency('gtk4'), dependency('jni'), dependency('glfw3')
                          ])

libtranslationlayer_so = shared_library('translation_layer_main', [
                                                                  	'src/api-impl-jni/egl/com_google_android_gles_jni_EGLImpl.c',
                                                                  	'src/api-impl-jni/android_os_Environment.c',
                                                                  	'src/api-impl-jni/android_os_SystemClock.c',
                                                                  	'src/api-impl-jni/android_view_Window.c',
                                                                  	'src/api-impl-jni/util.c',
                                                                  	'src/api-impl-jni/android_graphics_Canvas.c',
                                                                  	'src/api-impl-jni/drawables/ninepatch.c',
                                                                  	'src/api-impl-jni/android_content_res_AssetManager.c',
                                                                  	'src/api-impl-jni/audio/android_media_AudioTrack.c',
                                                                  	'src/api-impl-jni/widgets/android_widget_RelativeLayout.c',
                                                                  	'src/api-impl-jni/widgets/android_widget_ScrollView.c',
                                                                  	'src/api-impl-jni/widgets/android_opengl_GLSurfaceView.c',
                                                                  	'src/api-impl-jni/widgets/android_widget_ImageView.c',
                                                                  	'src/api-impl-jni/widgets/android_widget_FrameLayout.c',
                                                                  	'src/api-impl-jni/widgets/WrapperWidget.c',
                                                                  	'src/api-impl-jni/widgets/android_widget_TextView.c',
                                                                  	'src/api-impl-jni/widgets/android_widget_LinearLayout.c',
                                                                  	'src/api-impl-jni/widgets/android_view_SurfaceView.c',
                                                                  	'src/api-impl-jni/views/android_view_View.c',
                                                                  	'src/api-impl-jni/views/android_view_ViewGroup.c',
                                                                  	'src/api-impl-jni/android_graphics_Bitmap.c',
                                                                  	'src/api-impl-jni/android_app_NativeActivity.c',
                                                                  ] + marshal_files,
                                                                  install: true,
                                                                  install_dir : get_option('libdir') / 'java/dex/android_translation_layer/natives',
                                                                  dependencies: [
                                                                  	dependency('gtk4'), dependency('gl'), dependency('egl'), dependency('wayland-client'), dependency('jni')
                                                                  ],
                                                                  link_with: [ libandroid_so ],
                                                                  link_args: [
                                                                  	'-lasound'
                                                                  ])

executable('android-translation-layer', [ 
                   	'src/main-executable/main.c',
                   	'src/main-executable/r_debug.c'
                   ],
                   install: true,
                   dependencies: [
                   	dependency('gtk4'), dependency('jni'), declare_dependency(link_with: libtranslationlayer_so), libart_dep, dependency('dl'), libdl_bio_dep
                   ],
                   link_args: [
                   	'-rdynamic'
                   ])

# hax_arsc_parser.dex (named as classes2.dex so it works inside a jar)
subdir('src/arsc_parser')
hax_arsc_parser_dex = custom_target('hax_arsc_parser.dex', build_by_default: true, input: [hax_arsc_parser_jar], output: ['classes2.dex'],
                                    command: ['dx', '--verbose', '--dex', '--output='+join_paths(builddir_base, 'classes2.dex'), hax_arsc_parser_jar.full_path()])

# hax.dex (named as classes.dex so it works inside a jar)
subdir('src/api-impl')
hax_dex = custom_target('hax.dex', build_by_default: true, input: [hax_jar], output: ['classes.dex'],
                        command: ['dx', '--verbose', '--dex', '--output='+join_paths(builddir_base, 'classes.dex'), hax_jar.full_path()])

# api-impl.jar
custom_target('api-impl.jar', build_by_default: true, input: [hax_dex, hax_arsc_parser_dex], output: ['api-impl.jar'],
              install: true,
              install_dir : get_option('libdir') / 'java/dex/android_translation_layer',
              command: ['jar', '-cvf', join_paths(builddir_base, 'api-impl.jar'), '-C', builddir_base, hax_dex, '-C', builddir_base, hax_arsc_parser_dex])


