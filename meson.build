project('android_translation_layer', ['c', 'java'], default_options: ['b_lundef=false'])

incdir_dep = declare_dependency(include_directories: '.')
add_project_dependencies(incdir_dep, language: 'c')

cc = meson.get_compiler('c')
dir_base = meson.current_source_dir()
builddir_base = meson.current_build_dir()
libart_dep = [
	cc.find_library('art', dirs : [join_paths(dir_base, 'dalvik/linux-x86/lib64/')]),
     	cc.find_library('nativebridge', dirs : [join_paths(dir_base, 'dalvik/linux-x86/lib64/')])
]
libdl_bio_dep = [
	cc.find_library('dl_bio', dirs : [join_paths(dir_base, 'dalvik/linux-x86/lib64/')])
]

libtranslationlayer_so = shared_library('translation_layer_main', [
                                                             	'src/api-impl-jni/egl/com_google_android_gles_jni_EGLImpl.c',
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
                                                             	'src/api-impl-jni/views/android_view_View.c',
                                                             	'src/api-impl-jni/views/android_view_ViewGroup.c',
                                                             	'src/api-impl-jni/android_graphics_Bitmap.c'                                                             ],
                                                             dependencies: [
                                                             	dependency('gtk4'), dependency('gl'), dependency('egl'), dependency('jni')
                                                             ],
                                                             link_args: [
                                                             	'-lasound'
                                                             ])

conf_data = configuration_data()
conf_data.set('install_libdir', get_option('prefix') / get_option('libdir'))
configure_file(input : 'src/config.h.in',
               output : 'config.h',
               configuration : conf_data)

configure_file(input : 'launch_activity.sh.in',
               output : 'launch_activity.sh',
               configuration : conf_data)

executable('main', [ 
                   	'src/main-executable/main.c',
                   	'src/main-executable/r_debug.c'
                   ],
                   dependencies: [
                   	dependency('gtk4'), dependency('jni'), declare_dependency(link_with: libtranslationlayer_so), libart_dep, dependency('dl'), libdl_bio_dep
                   ])

# libandroid
shared_library('android', [
              	'src/libandroid/misc.c',
              	'src/libandroid/asset_manager.c'
              ])

# hax_arsc_parser.dex (named as classes2.dex so it works inside a jar)
subdir('src/arsc_parser')
hax_arsc_parser_dex = custom_target('hax_arsc_parser.dex', build_by_default: true, input: [hax_arsc_parser_jar], output: ['classes2.dex'], command: [join_paths(dir_base, 'dalvik/linux-x86/bin/dx'),'--verbose', '--dex', '--output='+join_paths(builddir_base, 'classes2.dex'), hax_arsc_parser_jar.full_path()])

# hax.dex (named as classes.dex so it works inside a jar)
subdir('src/api-impl')
hax_dex = custom_target('hax.dex', build_by_default: true, input: [hax_jar], output: ['classes.dex'], command: [join_paths(dir_base, 'dalvik/linux-x86/bin/dx'),'--verbose', '--dex', '--output='+join_paths(builddir_base, 'classes.dex'), hax_jar.full_path()])

# api-impl.jar
custom_target('api-impl.jar', build_by_default: true, input: [hax_dex, hax_arsc_parser_dex], output: ['api-impl.jar'], command: ['zip', '-j', join_paths(builddir_base, 'api-impl.jar'), hax_dex.full_path(), hax_arsc_parser_dex.full_path()])


