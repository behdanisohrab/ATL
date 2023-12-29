project('android_translation_layer', ['c', 'java'], default_options: ['b_lundef=false'])

javac = meson.get_compiler('java')
if javac.version() != '1.8.0'
	warning('javac versions other than 1.8.0 are known to cause issues at least on aarch64 alpine (you have: ', javac.version(), ')')
endif

gnome = import('gnome')
fs = import('fs')

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
libskia_dep = [
	cc.find_library('SkiaSharp')
]
if fs.is_file('/usr' / get_option('libdir') / 'java/core-all_classes.jar')
  bootclasspath = '/usr' / get_option('libdir') / 'java/core-all_classes.jar'
elif fs.is_file('/usr/local' / get_option('libdir') / 'java/core-all_classes.jar')
  bootclasspath = '/usr/local' / get_option('libdir') / 'java/core-all_classes.jar'
elif fs.is_file(get_option('prefix') / get_option('libdir') / 'java/core-all_classes.jar')
  bootclasspath = get_option('prefix') / get_option('libdir') / 'java/core-all_classes.jar'
else
  error('bootclasspath "core-all_classes.jar" not found')
endif

marshal_files = gnome.genmarshal('marshal',
	sources: 'src/api-impl-jni/widgets/marshal.list',
	valist_marshallers: true,
	internal: true,
)

wl_mod = import('unstable-wayland')

xml = wl_mod.find_protocol('linux-dmabuf', state: 'unstable', version: 1)
linux_dmabuf = wl_mod.scan_xml(xml)

xml = wl_mod.find_protocol('viewporter')
viewporter = wl_mod.scan_xml(xml)

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
                          	'src/libandroid/trace.c',
                          ],
                          install: true,
                          soversion: 0,
                          dependencies: [
                          	dependency('gtk4'), dependency('jni'), dependency('vulkan')
                          ])

libtranslationlayer_so = shared_library('translation_layer_main', [
                                                                  	'src/api-impl-jni/egl/com_google_android_gles_jni_EGLImpl.c',
                                                                  	'src/api-impl-jni/android_os_Environment.c',
                                                                  	'src/api-impl-jni/android_os_MessageQueue.c',
                                                                  	'src/api-impl-jni/android_os_SystemClock.c',
                                                                  	'src/api-impl-jni/android_view_Window.c',
                                                                  	'src/api-impl-jni/util.c',
                                                                  	'src/api-impl-jni/android_graphics_Canvas.c',
                                                                  	'src/api-impl-jni/android_graphics_Paint.c',
                                                                  	'src/api-impl-jni/android_util_Log.c',
                                                                  	'src/api-impl-jni/database/android_database_SQLiteCommon.c',
                                                                  	'src/api-impl-jni/database/android_database_SQLiteConnection.c',
                                                                  	'src/api-impl-jni/graphics/android_graphics_BitmapFactory.c',
                                                                  	'src/api-impl-jni/graphics/android_graphics_drawable_Drawable.c',
                                                                  	'src/api-impl-jni/graphics/android_graphics_drawable_DrawableContainer.c',
                                                                  	'src/api-impl-jni/graphics/android_graphics_Typeface.c',
                                                                  	'src/api-impl-jni/media/android_media_MediaCodec.c',
                                                                  	'src/api-impl-jni/android_content_res_AssetManager.c',
                                                                  	'src/api-impl-jni/audio/android_media_AudioTrack.c',
                                                                  	'src/api-impl-jni/audio/android_media_SoundPool.c',
                                                                  	'src/api-impl-jni/widgets/android_widget_Button.c',
                                                                  	'src/api-impl-jni/widgets/android_widget_EditText.c',
                                                                  	'src/api-impl-jni/widgets/android_widget_ImageButton.c',
                                                                  	'src/api-impl-jni/widgets/android_widget_ScrollView.c',
                                                                  	'src/api-impl-jni/widgets/android_widget_ImageView.c',
                                                                  	'src/api-impl-jni/widgets/WrapperWidget.c',
                                                                  	'src/api-impl-jni/widgets/android_widget_TextView.c',
                                                                  	'src/api-impl-jni/widgets/android_widget_LinearLayout.c',
                                                                  	'src/api-impl-jni/widgets/android_widget_Progressbar.c',
                                                                  	'src/api-impl-jni/widgets/android_view_SurfaceView.c',
                                                                  	'src/api-impl-jni/views/AndroidLayout.c',
                                                                  	'src/api-impl-jni/views/android_view_View.c',
                                                                  	'src/api-impl-jni/views/android_view_ViewGroup.c',
                                                                  	'src/api-impl-jni/android_graphics_Bitmap.c',
                                                                  	'src/api-impl-jni/android_app_NativeActivity.c',
                                                                  	'src/api-impl-jni/android_opengl_GLES20.c',
                                                                  	'src/api-impl-jni/location/android_location_LocationManager.c',
                                                                  	'src/api-impl-jni/app/android_app_Activity.c',
                                                                  	'src/api-impl-jni/app/android_app_AlertDialog.c',
                                                                  	'src/api-impl-jni/app/android_app_Dialog.c',
                                                                  	'src/sk_area/sk_area.c',
																	linux_dmabuf,
																	viewporter,
                                                                  ] + marshal_files,
                                                                  include_directories: ['src/sk_area/'],
                                                                  install: true,
                                                                  install_dir : get_option('libdir') / 'java/dex/android_translation_layer/natives',
                                                                  dependencies: [
                                                                  	dependency('gtk4', version: '>=4.8'), dependency('gl'), dependency('egl'), dependency('wayland-client'), dependency('jni'),
                                                                  	dependency('libportal'), dependency('sqlite3'), libskia_dep, dependency('libavcodec', version: '>=59'), dependency('libdrm')
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
                   ],
                   install_rpath: get_option('prefix') / get_option('libdir') / 'art:' + get_option('prefix') / get_option('libdir') / 'java/dex/android_translation_layer/natives')

# hax_arsc_lib.dex (named as classes2.dex so it works inside a jar)
subdir('src/ARSCLib')
hax_arsc_lib_dex = custom_target('hax_arsc_lib.dex', build_by_default: true, input: [hax_arsc_lib_jar], output: ['classes2.dex'],
                                    command: ['dx', '--verbose', '--dex', '--min-sdk-version', '26', '--output='+join_paths(builddir_base, 'classes2.dex'), hax_arsc_lib_jar.full_path()])

# hax.dex (named as classes.dex so it works inside a jar)
subdir('src/api-impl')
hax_dex = custom_target('hax.dex', build_by_default: true, input: [hax_jar], output: ['classes.dex'],
                        command: ['dx', '--verbose', '--dex', '--output='+join_paths(builddir_base, 'classes.dex'), hax_jar.full_path()])

# api-impl.jar
custom_target('api-impl.jar', build_by_default: true, input: [hax_dex, hax_arsc_lib_dex], output: ['api-impl.jar'],
              install: true,
              install_dir : get_option('libdir') / 'java/dex/android_translation_layer',
              command: ['jar', '-cvf', join_paths(builddir_base, 'api-impl.jar'), '-C', builddir_base, hax_dex, '-C', builddir_base, hax_arsc_lib_dex])

#framework-res.apk
subdir('res')

