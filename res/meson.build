aapt = find_program('aapt', required: false)

if aapt.found()
  aapt_command = [aapt, 'package', '-x', '-f', '-S', join_paths(dir_base, 'res/res'), '-A', join_paths(dir_base, 'res/assets'), '-M', join_paths(dir_base, 'res/AndroidManifest.xml'), '-J', join_paths(dir_base, 'src/api-impl/com/android/internal'), '-F', '@OUTPUT@']
else  # fallback to prebuilt APK
  aapt_command = ['cp', join_paths(dir_base, 'res/framework-res.apk'), '@OUTPUT@']
endif

  framework_res = custom_target('framework-res.apk',
    output : 'framework-res.apk',
    command : aapt_command,
    install : true,
    install_dir : get_option('libdir') / 'java/dex/android_translation_layer')
