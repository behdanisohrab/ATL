#define GL_GLEXT_PROTOTYPES

#include <GL/gl.h>
#include <GL/glcorearb.h>

#include <EGL/egl.h>
#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>

#include <gtk/gtk.h>

#include "../defines.h"
#include "../util.h"

#include "WrapperWidget.h"

#include "../generated_headers/android_opengl_GLSurfaceView.h"

// for whatever reason, some Mesa builds don't export the OES function (which we use in order to have GLESv1 support)
GL_APICALL void GL_APIENTRY _glEGLImageTargetTexture2DOES_load(GLenum target, GLeglImageOES image);
static PFNGLEGLIMAGETARGETTEXTURE2DOESPROC _glEGLImageTargetTexture2DOES = &_glEGLImageTargetTexture2DOES_load;

GL_APICALL void GL_APIENTRY _glEGLImageTargetTexture2DOES_load(GLenum target, GLeglImageOES image)
{
	_glEGLImageTargetTexture2DOES =
		(PFNGLEGLIMAGETARGETTEXTURE2DOESPROC)eglGetProcAddress("glEGLImageTargetTexture2DOES");

	_glEGLImageTargetTexture2DOES(target, image);
}

GL_APICALL void GL_APIENTRY __attribute__((weak)) glEGLImageTargetTexture2DOES(GLenum target, GLeglImageOES image)
{
	_glEGLImageTargetTexture2DOES(target, image);
}
// end of OES workaround

//#define FIXME__WIDTH 540
//#define FIXME__HEIGHT 960

// FIXME: what do we do here? we should probably take these from the size of the GLArea, but how do we change the sizes of all the textures and buffers etc when the GLArea changes size?
// for now, borrow the initial app window width/height
extern int FIXME__WIDTH;
extern int FIXME__HEIGHT;

// mainly frame markers (very obtrusive, not recommended for most builds)
#ifdef DEBUG_GLAREA
#define d_printf(...) printf(VA_ARGS)
#else
#define d_printf(...)
#endif

#define ___GL_TRACE___(message) glDebugMessageInsert(GL_DEBUG_SOURCE_THIRD_PARTY, GL_DEBUG_TYPE_MARKER, 0xdeadbeef, GL_DEBUG_SEVERITY_NOTIFICATION, strlen(message), message)

JNIEXPORT void JNICALL Java_android_opengl_GLSurfaceView_native_1constructor__Landroid_content_Context_2(JNIEnv *env, jobject this, jobject context)
{
	GtkWidget *wrapper = wrapper_widget_new();
	GtkWidget *gl_area = gtk_gl_area_new();
	wrapper_widget_set_child(WRAPPER_WIDGET(wrapper), gl_area);
	_SET_LONG_FIELD(this, "widget", _INTPTR(gl_area));
}

//two triangles put together to make a square
const float positions[] = {
	-1, -1, 0,
	-1,  1, 0,
	 1, -1, 0,
	 1, -1, 0,
	-1,  1, 0,
	 1,  1, 0
};

const char *vertexStr = "#version 320 es\n"
                        "in vec3 pos;\n"
                        "out vec2 texCoords;\n"
                        "void main(){\n"
                        "   texCoords = vec2(pos.x, pos.y);\n"
                        "   gl_Position = vec4((pos.x * 2.0) - 1.0, (pos.y * 2.0) - 1.0, pos.z, 1.0);\n"
                        "}\n";

const char *fragmentStr = "#version 320 es\n"
                          "out highp vec4 outputColor;\n"
                          "in highp vec2 texCoords;\n"
                          "uniform sampler2D texSampler;\n"
                          "void main(){\n"
                          "   outputColor = texture(texSampler, texCoords);\n"
                          "}\n";

struct render_priv {
	GLuint program;
	GLuint vao;
	int texUnit;
	GLuint texBufferdObject;
	GLuint sampler;

/* --- */
    EGLSurface eglSurface;
    EGLContext eglContext;

	GLuint FramebufferName;
	GLuint renderedTexture;
	GLuint texture_id;

	EGLImage egl_image;
};

static void check_shader_compile_error(GLuint shader)
{
	GLint compileResult;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compileResult);

	if(compileResult != GL_TRUE) {
		GLint log_size = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_size);

		GLchar *info_log = malloc(log_size);
		glGetShaderInfoLog(shader, log_size, &log_size, info_log);
		fprintf(stderr, "\nError compiling one of the shaders used by GLSurfaceView:\n%s\n---\nsince this error shouldn't ever happen, we fail hard\n", info_log);
		free(info_log);

		exit(1);
	}
}

static void check_program_link_error(GLuint program)
{
	GLint link_status = 0;
	glGetProgramiv(program, GL_LINK_STATUS, &link_status);

	if(link_status != GL_TRUE) {
		GLint log_size = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_size);

		GLchar *info_log = malloc(log_size);
		glGetProgramInfoLog(program, log_size, &log_size, info_log);
		fprintf(stderr, "\nError linking the program used by GLSurfaceView:\n%s\n---\nsince this error shouldn't ever happen, we fail hard\n", info_log);
		free(info_log);

		exit(1);
	}
}

#define check_egl_error()                                                                                                         \
	do {                                                                                                                      \
		EGLint error_ = eglGetError();                                                                                    \
		if(error_ != EGL_SUCCESS)                                                                                         \
			fprintf(stderr, "\nError in %s (%s:%d): \n"                                                               \
				        "eglGetError reported 0x%x\n"                                                             \
				        "this might be because we need to implement more stuff, so we will continue from here\n", \
				        __func__, __FILE__, __LINE__, error_);                                                    \
	} while(0)

// TODO: use this where appropriate
#define check_gl_error()                                                                                                          \
	do {                                                                                                                      \
		GLenum error_ = glGetError();                                                                                     \
		if(error_ != GL_NO_ERROR)                                                                                         \
			fprintf(stderr, "\nError in %s (%s:%d): \n"                                                               \
				        "glGetError reported 0x%x\n"                                                              \
				        "this might be because we need to implement more stuff, so we will continue from here\n", \
				        __func__, __FILE__, __LINE__, error_);                                                    \
	} while(0)


struct jni_gl_callback_data { JavaVM *jvm; jobject this; jobject renderer; bool first_time;};
static void on_realize(GtkGLArea *gl_area, struct jni_gl_callback_data *d)
{
	gtk_gl_area_make_current(gl_area);

	struct render_priv *render_priv = g_object_get_data(G_OBJECT(gl_area), "render_priv");

	JNIEnv *env;
	(*d->jvm)->GetEnv(d->jvm, (void**)&env, JNI_VERSION_1_6);

	EGLDisplay eglDisplay = eglGetDisplay((EGLNativeDisplayType)0);
	EGLDisplay old_eglDisplay = eglGetCurrentDisplay();

	d_printf("GTK version: >%d__%d__%d<\n", gtk_get_major_version(), gtk_get_minor_version(), gtk_get_micro_version());
	d_printf("OpenGL version: >%s<\n", glGetString(GL_VERSION));

	glGenTextures(1, &render_priv->texture_id);

	// vertex shader

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexStr, NULL);
	glCompileShader(vertexShader);

	check_shader_compile_error(vertexShader);

	// fragment shader

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentStr, NULL);
	glCompileShader(fragmentShader);

	check_shader_compile_error(fragmentShader);

	// program

	render_priv->program = glCreateProgram();

	glAttachShader(render_priv->program, vertexShader);
	glAttachShader(render_priv->program, fragmentShader);
	glLinkProgram(render_priv->program);

	check_program_link_error(render_priv->program);

	glUseProgram(render_priv->program);

	// the triangles

	glGenVertexArrays(1, &render_priv->vao);
	glBindVertexArray(render_priv->vao);

	GLuint positionBufferObject;
	glGenBuffers(1, &positionBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, positionBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*6*3,positions, GL_STREAM_DRAW);

	GLuint posIndex = glGetAttribLocation(render_priv->program, "pos");
	glEnableVertexAttribArray(posIndex);
	glVertexAttribPointer(posIndex, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glUseProgram(0);

	glUseProgram(render_priv->program);

	// the texture we will be rendering to

	glGenTextures(1, &render_priv->texBufferdObject);
	glBindTexture(GL_TEXTURE_2D, render_priv->texBufferdObject);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	// texture sampler for our triangles

	glGenSamplers(1, &render_priv->sampler);
	glSamplerParameteri(render_priv->sampler, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glSamplerParameteri(render_priv->sampler, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glSamplerParameteri(render_priv->sampler, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	GLuint samplerUniform = glGetUniformLocation(render_priv->program, "texSampler");
	glUniform1i(samplerUniform, render_priv->texUnit);
	glUseProgram(0);

	// EGL setup

	eglInitialize(eglDisplay, 0, 0);
	eglBindAPI(EGL_OPENGL_ES_API);

	EGLConfig eglConfig;

	// a roundabout way to let the app define the parameter list, and then choose the best fit out of all the returned configs
	eglConfig = (EGLConfig)_PTR((*env)->CallLongMethod(env, d->this, handle_cache.gl_surface_view.wrap_EGLConfigChooser_chooseConfig, _INTPTR(eglDisplay)));
	if((*env)->ExceptionCheck(env))
		(*env)->ExceptionDescribe(env);
	check_egl_error();

	// set up the pbuffer (TODO: is there any way to dynamically change the width/height?)

	EGLint pbufferAttribs[5];
	pbufferAttribs[0] = EGL_WIDTH;
	pbufferAttribs[1] = FIXME__WIDTH;
	pbufferAttribs[2] = EGL_HEIGHT;
	pbufferAttribs[3] = FIXME__HEIGHT;
	pbufferAttribs[4] = EGL_NONE;

	render_priv->eglSurface = eglCreatePbufferSurface(eglDisplay, eglConfig, pbufferAttribs);
	check_egl_error();

	// a roundabout way to run eglCreateContext with the atrribute list that the app chose
	render_priv->eglContext = (EGLContext)_PTR((*env)->CallLongMethod(env, d->this, handle_cache.gl_surface_view.wrap_EGLContextFactory_createContext, _INTPTR(eglDisplay), _INTPTR(eglConfig)));
	check_egl_error();

	// save the EGL state before we change it, so we can switch back later
	EGLContext old_eglContext = eglGetCurrentContext();
	EGLSurface old_read_surface = eglGetCurrentSurface(EGL_READ);
	EGLSurface old_draw_surface = eglGetCurrentSurface(EGL_DRAW);

	bool result = eglMakeCurrent(eglDisplay, render_priv->eglSurface, render_priv->eglSurface, render_priv->eglContext);
	check_egl_error();

	// set up the framebuffer
	glGenFramebuffers(1, &render_priv->FramebufferName);
	glBindFramebuffer(GL_FRAMEBUFFER, render_priv->FramebufferName);
	check_gl_error();

	glGenTextures(1, &render_priv->renderedTexture);
	glBindTexture(GL_TEXTURE_2D, render_priv->renderedTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, FIXME__WIDTH, FIXME__HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
	check_gl_error();

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	check_gl_error();

	GLuint depthrenderbuffer;
	glGenRenderbuffers(1, &depthrenderbuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthrenderbuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, FIXME__WIDTH, FIXME__HEIGHT);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthrenderbuffer);
	check_gl_error();

	// Set "renderedTexture" as our colour attachement #0
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, render_priv->renderedTexture, 0);
	check_gl_error();

	// Set the list of draw buffers.
	GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
	glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers
	check_gl_error();

	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		fprintf(stderr, "Error: glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE\n");

	// create the EGL Image which we will use to access the rendered-to texture from Gtk's EGL context
	render_priv->egl_image = eglCreateImage(eglDisplay, render_priv->eglContext, EGL_GL_TEXTURE_2D, (EGLClientBuffer)(intptr_t)render_priv->renderedTexture, NULL);
	check_egl_error();

	// the GLSurfaceView implements SurfaceHolder.Callback, and the app may expect that we call the `surfaceCreated` method
	(*env)->CallVoidMethod(env, d->this, handle_cache.surface_holder_callback.surfaceCreated, NULL);
	if((*env)->ExceptionCheck(env))
		(*env)->ExceptionDescribe(env);

	// Here we call the app's onSurfaceCreated callback. This is the android API's equivalent of the `realize` callback that we are currently in.
	___GL_TRACE___("---- calling onSurfaceCreated");
	(*env)->CallVoidMethod(env, d->renderer, handle_cache.renderer.onSurfaceCreated, _GET_OBJ_FIELD(d->this, "java_gl_wrapper", "Ljavax/microedition/khronos/opengles/GL10;"), NULL); // FIXME passing NULL only works if the app doesn't use these parameters
	if((*env)->ExceptionCheck(env))
		(*env)->ExceptionDescribe(env);
	___GL_TRACE___("---- returned from calling onSurfaceCreated");

	// This should be called from Gtk's `resize` callback. However, until we figure out how to resize the pbuffer, the framebuffer, and the texture, we can't afford to pass the actual widget's dimensions here
	___GL_TRACE___("---- calling onSurfaceChanged");
	(*env)->CallVoidMethod(env, d->renderer, handle_cache.renderer.onSurfaceChanged, NULL, FIXME__WIDTH, FIXME__HEIGHT); // FIXME put this in the right callback (and pass the actual dimensions)
	if((*env)->ExceptionCheck(env))
		(*env)->ExceptionDescribe(env);
	___GL_TRACE___("---- returned from calling onSurfaceChanged");

	// restore the EGL context so that Gtk doesn't get confused

	result = eglMakeCurrent(old_eglDisplay, old_read_surface, old_draw_surface, old_eglContext);
	check_egl_error();
}

static gboolean render(GtkGLArea *gl_area, GdkGLContext *context, struct jni_gl_callback_data *d)
{
	struct render_priv *render_priv = g_object_get_data(G_OBJECT(gl_area), "render_priv");

	JNIEnv *env;
	(*d->jvm)->GetEnv(d->jvm, (void**)&env, JNI_VERSION_1_6);

	EGLDisplay eglDisplay = eglGetDisplay((EGLNativeDisplayType)0);
	EGLDisplay old_eglDisplay = eglGetCurrentDisplay();

	// save the EGL state before we change it, so we can switch back later
	EGLContext old_eglContext = eglGetCurrentContext();
	EGLSurface old_read_surface = eglGetCurrentSurface(EGL_READ);
	EGLSurface old_draw_surface = eglGetCurrentSurface(EGL_DRAW);

	bool result = eglMakeCurrent(eglDisplay, render_priv->eglSurface, render_priv->eglSurface, render_priv->eglContext);
	check_egl_error();

	// bind the framebuffer that we are rendering into
	check_gl_error();
	glBindFramebuffer(GL_FRAMEBUFFER, render_priv->FramebufferName);
	check_gl_error();
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, render_priv->renderedTexture, 0);
	check_gl_error();

	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		fprintf(stderr, "Error: glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE\n");

	d_printf("OpenGL version before calling onDrawFrame: >%s<\n", glGetString(GL_VERSION));
	d_printf("\n\n\n---- calling onDrawFrame\n\n");
	// this marks the part where the app is doing the rendering (as opposed to Gtk) for easier orientation in a trace (e.g apitrace)
	// TODO: make a program that extracts just these fragments from a trace
	// TODO: add a unique identifier of this GLArea (worst case the pointer to this widget)
check_gl_error();
	___GL_TRACE___("---- calling onDrawFrame");

	// Here we call the app's onDrawFrame callback. This is the android API's equivalent of the `render` callback that we are currently in.
	(*env)->CallVoidMethod(env, d->renderer, handle_cache.renderer.onDrawFrame, NULL); // FIXME passing NULL only works if the app doesn't use this parameter
	if((*env)->ExceptionCheck(env))
		(*env)->ExceptionDescribe(env);

	___GL_TRACE___("---- returned from calling onDrawFrame");
check_gl_error();
	d_printf("\n---- returned from calling onDrawFrame\n\n\n\n");

	eglSwapBuffers(eglDisplay, render_priv->eglSurface);

	// switch the EGL context back to Gtk's one
	result = eglMakeCurrent(old_eglDisplay, old_read_surface, old_draw_surface, old_eglContext);
	check_egl_error();

	gtk_gl_area_make_current(gl_area);

	d_printf("OpenGL version after restore: >%s<\n", glGetString(GL_VERSION));
	d_printf("\n\n\n---- drawing the texture containing our frame\n\n");
	___GL_TRACE___("---- drawing the texture containing our frame");

	// draw on a black background (TODO: isn't this pointless?)
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// draw two triangles (a rectangle) painted with the texture that we were rendering into
	glUseProgram(render_priv->program);
	glBindVertexArray(render_priv->vao);

	glActiveTexture(GL_TEXTURE0 + render_priv->texUnit);
	glBindTexture(GL_TEXTURE_2D, render_priv->texBufferdObject);

	glEGLImageTargetTexture2DOES(GL_TEXTURE_2D, render_priv->egl_image);
	glBindSampler(render_priv->texUnit, render_priv->sampler);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);

	___GL_TRACE___("---- end of drawing the texture containing our frame");
	d_printf("\n---- end of drawing the texture containing our frame\n\n\n\n");

	return TRUE;
}

struct jni_callback_data { JavaVM *jvm; jobject this; jclass this_class; };

static void call_ontouch_callback(GtkEventControllerLegacy* event_controller, int action, double x, double y, struct jni_callback_data *d)
{
	JNIEnv *env;
	(*d->jvm)->GetEnv(d->jvm, (void**)&env, JNI_VERSION_1_6);

	// translate to the GLSurfaceArea widget's coordinates, since that's what the app expects

	double off_x;
	double off_y;

	GtkWidget *gl_area = gtk_event_controller_get_widget(event_controller);
	GtkWidget *window = GTK_WIDGET(gtk_widget_get_native(gl_area));

	// compensate for offset between the widget coordinates and the surface coordinates
	gtk_native_get_surface_transform(GTK_NATIVE(window), &off_x, &off_y);
	x -= off_x;
	y -= off_y;
	gtk_widget_translate_coordinates(window, gl_area, x, y, &x, &y);

	// execute the Java callback function

	jobject motion_event = (*env)->NewObject(env, handle_cache.motion_event.class, handle_cache.motion_event.constructor, action, (float)x, (float)y);

	(*env)->CallBooleanMethod(env, d->this, handle_cache.gl_surface_view.onTouchEvent, motion_event);

	if((*env)->ExceptionCheck(env))
		(*env)->ExceptionDescribe(env);

	(*env)->DeleteLocalRef(env, motion_event);
}

// TODO: find a way to reconcile this with libandroid/input.c
static gboolean on_event(GtkEventControllerLegacy* self, GdkEvent* event, struct jni_callback_data *d)
{
	double x;
	double y;

	// TODO: this doesn't work for multitouch
	switch(gdk_event_get_event_type(event)) {
		case GDK_BUTTON_PRESS:
		case GDK_TOUCH_BEGIN:
			gdk_event_get_position(event, &x, &y);
			call_ontouch_callback(self, MOTION_EVENT_ACTION_DOWN, x, y, d);
			break;
		case GDK_BUTTON_RELEASE:
		case GDK_TOUCH_END:
			gdk_event_get_position(event, &x, &y);
			call_ontouch_callback(self, MOTION_EVENT_ACTION_UP, x, y, d);
			break;
		case GDK_MOTION_NOTIFY:
		case GDK_TOUCH_UPDATE:
			gdk_event_get_position(event, &x, &y);
			call_ontouch_callback(self, MOTION_EVENT_ACTION_MOVE, x, y, d);
			break;
	}
}

extern gboolean tick_callback(GtkWidget* widget, GdkFrameClock* frame_clock, gpointer user_data);

JNIEXPORT void JNICALL Java_android_opengl_GLSurfaceView_native_1set_1renderer(JNIEnv *env, jobject this, jobject renderer, jboolean implements_onTouchEvent)
{
	GtkWidget *gl_area = GTK_WIDGET(_PTR(_GET_LONG_FIELD(this, "widget")));

	gtk_gl_area_set_has_stencil_buffer(GTK_GL_AREA(gl_area), true); // FIXME don't assume what the app wants
	gtk_gl_area_set_has_depth_buffer(GTK_GL_AREA(gl_area), true);

	gtk_gl_area_set_use_es(GTK_GL_AREA(gl_area), true); // FIXME

	struct render_priv *render_priv = malloc(sizeof(struct render_priv));
	render_priv->texUnit = 0;
	render_priv->sampler = 0;
	render_priv->FramebufferName = 0;

	g_object_set_data(G_OBJECT(gl_area), "render_priv", render_priv);

	JavaVM *jvm;
	(*env)->GetJavaVM(env, &jvm);

	struct jni_gl_callback_data *gl_callback_data = malloc(sizeof(struct jni_gl_callback_data));
	gl_callback_data->jvm = jvm;
	gl_callback_data->this = _REF(this);
	gl_callback_data->renderer = _REF(renderer);
	gl_callback_data->first_time = 1;

	g_signal_connect(gl_area, "render", G_CALLBACK(render), gl_callback_data);
	g_signal_connect(gl_area, "realize", G_CALLBACK(on_realize), gl_callback_data);

	if(implements_onTouchEvent) {
		struct jni_callback_data *callback_data = malloc(sizeof(struct jni_callback_data));
		callback_data->jvm = jvm;
		callback_data->this = _REF(this);
		callback_data->this_class = _REF(_CLASS(this));

		printf("callback_data->jvm: %p\n", callback_data->jvm);

		GtkEventController *controller = gtk_event_controller_legacy_new();
		gtk_widget_add_controller(gl_area, controller);
		g_signal_connect(controller, "event", G_CALLBACK(on_event), callback_data);

		printf("GREP FOR MEEEE -- //FIXME\n");
	}

	gtk_widget_add_tick_callback(gl_area, tick_callback, NULL, NULL);

	//FIXME
	gtk_widget_set_hexpand(gtk_widget_get_parent(GTK_WIDGET(gl_area)), true);
}
