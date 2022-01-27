#define GL_GLEXT_PROTOTYPES

#include <GL/gl.h>
#include <GL/glcorearb.h>

#include <EGL/egl.h>
#include <GLES/gl.h>
#include <GLES/glext.h>

#include <gtk/gtk.h>

#include "../defines.h"
#include "../util.h"

#include "WrapperWidget.h"

#include "android_opengl_GLSurfaceView.h"

//#define FIXME__WIDTH 540
//#define FIXME__HEIGHT 960

#define FIXME__WIDTH 960
#define FIXME__HEIGHT 540


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

struct jni_gl_callback_data { JavaVM *jvm; jobject renderer; bool first_time;};
static gboolean render(GtkGLArea *area, GdkGLContext *context, struct jni_gl_callback_data *d)
{

	static GLuint program;
	static GLuint vao;
	static const int texUnit=0;
	static GLuint texBufferdObject;
	static GLuint sampler=0;

/* --- */
    static EGLSurface eglSurface;
    static EGLContext eglContext;

	static GLuint FramebufferName = 0;
	static GLuint renderedTexture;
	static GLuint texture_id;

	static EGLImage egl_image;
/* --- */

/* --- */
	printf("WEEEE AAAAARE HEEEEERE\n");
	printf("d: %p\n", d);
	printf("jvm: %p\n", d->jvm);

	JNIEnv *env;
	(*d->jvm)->GetEnv(d->jvm, (void**)&env, JNI_VERSION_1_6);

	printf("WEEEE AAAAARE HEEEEERE TWO\n");

    EGLDisplay eglDisplay = eglGetDisplay((EGLNativeDisplayType)0);
    EGLDisplay old_eglDisplay = eglGetCurrentDisplay();
/* --- */

	if(d->first_time) {
/* --- */
		printf("GTK version: >%d__%d__%d<\n", gtk_get_major_version(), gtk_get_minor_version(), gtk_get_micro_version());
		printf("OpenGL version: >%s<\n", glGetString(GL_VERSION));

		glGenTextures(1, &texture_id);
/* --- */

		// TODO - proper error checking

		GLuint vertexShader = glCreateShader( GL_VERTEX_SHADER );
		glShaderSource(vertexShader, 1, &vertexStr, NULL);
		glCompileShader( vertexShader );

/*
GLint logSize = 0;
glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &logSize);

GLchar *infoLog = malloc(logSize);
glGetShaderInfoLog(vertexShader, logSize, &logSize, infoLog);
printf("\n...>%s<\n\n", infoLog);
free(infoLog);
*/
		GLuint fragmentShader = glCreateShader( GL_FRAGMENT_SHADER );
		glShaderSource(fragmentShader, 1, &fragmentStr, NULL);
		glCompileShader( fragmentShader );
/*
glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &logSize);

infoLog = malloc(logSize);
glGetShaderInfoLog(fragmentShader, logSize, &logSize, infoLog);
printf("\n...>%s<\n\n", infoLog);
free(infoLog);
*/
		program = glCreateProgram();

		glAttachShader(program, vertexShader);
		glAttachShader(program, fragmentShader);
		glLinkProgram(program);
/*
	GLint maxLength = 0;
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);
	infoLog = malloc(maxLength);

	glGetProgramInfoLog(program, maxLength, &maxLength, infoLog);

	printf("\n..>%s<\n\n", infoLog);
*/

		glUseProgram(program);

		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		GLuint positionBufferObject;
		glGenBuffers(1, &positionBufferObject);
		glBindBuffer(GL_ARRAY_BUFFER, positionBufferObject);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float)*6*3,positions, GL_STREAM_DRAW);

		GLuint posIndex = glGetAttribLocation(program, "pos");
		glEnableVertexAttribArray(posIndex);
		glVertexAttribPointer(posIndex, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		glUseProgram(0);

		glUseProgram(program);

		glGenTextures(1, &texBufferdObject);
		glBindTexture(GL_TEXTURE_2D, texBufferdObject);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
		glBindTexture(GL_TEXTURE_2D, 0);

		glGenSamplers(1, &sampler);
		glSamplerParameteri(sampler, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glSamplerParameteri(sampler, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glSamplerParameteri(sampler, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		GLuint samplerUniform = glGetUniformLocation(program, "texSampler");
		glUniform1i(samplerUniform, texUnit);
		glUseProgram(0);

/* --- */
		eglInitialize(eglDisplay, 0, 0);
		eglBindAPI(EGL_OPENGL_ES_API);

		printf("eglDisplay: >%p<\n", eglDisplay);

		// Step 4 - Specify the required configuration attributes.
		EGLint pi32ConfigAttribs[17];
		pi32ConfigAttribs[0] = EGL_SURFACE_TYPE;
		pi32ConfigAttribs[1] = EGL_PBUFFER_BIT;
		pi32ConfigAttribs[2] = EGL_RENDERABLE_TYPE;
		pi32ConfigAttribs[3] = EGL_OPENGL_ES_BIT;
		pi32ConfigAttribs[4] = EGL_RED_SIZE;
		pi32ConfigAttribs[5] = 8;
		pi32ConfigAttribs[6] = EGL_GREEN_SIZE;
		pi32ConfigAttribs[7] = 8;
		pi32ConfigAttribs[8] = EGL_BLUE_SIZE;
		pi32ConfigAttribs[9] = 8;
		pi32ConfigAttribs[10] = EGL_ALPHA_SIZE;
		pi32ConfigAttribs[11] = 0;
		pi32ConfigAttribs[12] = EGL_DEPTH_SIZE;
		pi32ConfigAttribs[13] = 16;
		pi32ConfigAttribs[14] = EGL_STENCIL_SIZE;
		pi32ConfigAttribs[15] = 0;
		pi32ConfigAttribs[16] = EGL_NONE;

		printf("have not crashed yet 1\n");

		int iConfigs;
		EGLConfig eglConfig;
		printf("have not crashed yet 2\n");
		eglChooseConfig(eglDisplay, pi32ConfigAttribs, &eglConfig, 1, &iConfigs);
		printf("have not crashed yet 3\n");

		printf("iConfigs: >%d<\n", iConfigs); // SUS
		printf("eglConfig: >%p<\n", eglConfig);

		EGLint pbufferAttribs[5];
		pbufferAttribs[0] = EGL_WIDTH;
		pbufferAttribs[1] = FIXME__WIDTH;
		pbufferAttribs[2] = EGL_HEIGHT;
		pbufferAttribs[3] = FIXME__HEIGHT;
		pbufferAttribs[4] = EGL_NONE;

		eglSurface = eglCreatePbufferSurface(eglDisplay, eglConfig, pbufferAttribs);

		printf("eglCreatePbufferSurface: error: >%d<\n", eglGetError());

		printf("eglSurface: %d, EGL_NO_SURFACE: %d\n", eglSurface, EGL_NO_SURFACE);

		EGLint ai32ContextAttribs[] = { EGL_CONTEXT_CLIENT_VERSION, 1, EGL_NONE };

		eglContext = eglCreateContext(eglDisplay, eglConfig, NULL, ai32ContextAttribs);

		printf("eglContext: %d, EGL_NO_CONTEXT: %d\n", eglContext, EGL_NO_CONTEXT);

		EGLContext old_eglContext = eglGetCurrentContext();
		EGLSurface old_read_surface = eglGetCurrentSurface(EGL_READ);
		EGLSurface old_draw_surface = eglGetCurrentSurface(EGL_DRAW);

		bool result = eglMakeCurrent(eglDisplay, eglSurface, eglSurface, eglContext);
		printf("result: %d, error:>%d<\n", result, eglGetError());

		glGenFramebuffers(1, &FramebufferName);
		glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);

		glGenTextures(1, &renderedTexture);
		glBindTexture(GL_TEXTURE_2D, renderedTexture);
		glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, FIXME__WIDTH, FIXME__HEIGHT, 0,GL_RGB,   GL_UNSIGNED_BYTE, 0);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		GLuint depthrenderbuffer;
		glGenRenderbuffers(1, &depthrenderbuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, depthrenderbuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, FIXME__WIDTH, FIXME__HEIGHT);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthrenderbuffer);

		// Set "renderedTexture" as our colour attachement #0
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderedTexture, 0);

		// Set the list of draw buffers.
		GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
		glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers

		if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			printf("glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE - REEEEEEEE!");

		printf("FRAMEBUFFER stuff error:>%d<\n", eglGetError());

		egl_image = eglCreateImage(eglDisplay, eglContext, EGL_GL_TEXTURE_2D, (EGLClientBuffer)(uint32_t)renderedTexture, NULL);
		printf("eglCreateImage: error: %d\n", eglGetError());
/* --- */

/* ... */
		(*env)->CallVoidMethod(env, d->renderer, handle_cache.renderer.onSurfaceCreated, NULL, NULL); // FIXME passing NULL only works if the app doesn't use these parameters
		if((*env)->ExceptionCheck(env))
			(*env)->ExceptionDescribe(env);

		(*env)->CallVoidMethod(env, d->renderer, handle_cache.renderer.onSurfaceChanged, NULL, FIXME__WIDTH, FIXME__HEIGHT); // FIXME put this in the right callback (and pass the actual dimensions)
		if((*env)->ExceptionCheck(env))
			(*env)->ExceptionDescribe(env);
/* ... */

		result = eglMakeCurrent(old_eglDisplay, old_read_surface, old_draw_surface, old_eglContext);
		printf("restore result: %d\n", result);

		d->first_time = 0;
	}
/* --- */
	EGLContext old_eglContext = eglGetCurrentContext();
	EGLSurface old_read_surface = eglGetCurrentSurface(EGL_READ);
	EGLSurface old_draw_surface = eglGetCurrentSurface(EGL_DRAW);

	bool result = eglMakeCurrent(eglDisplay, eglSurface, eglSurface, eglContext);
	printf("eglMakeCurrent for our context returned: %d\n", result);

	glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderedTexture, 0);

	printf("glBindFramebuffer: error: %d\n", eglGetError());

	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		printf("2: glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE - REEEEEEEE! (==: %d)", glCheckFramebufferStatus(GL_FRAMEBUFFER));

	printf("glCheckFramebufferStatus: error: %d\n", eglGetError());


	printf("OpenGL version before calling onDrawFrame: >%s<\n", glGetString(GL_VERSION));
	printf("\n\n\n---- calling onDrawFrame\n\n");
	glDebugMessageInsert(GL_DEBUG_SOURCE_THIRD_PARTY, GL_DEBUG_TYPE_MARKER, 0xdeadbeef, GL_DEBUG_SEVERITY_NOTIFICATION, strlen("---- calling onDrawFrame"), "---- calling onDrawFrame");
/* ... */
	(*env)->CallVoidMethod(env, d->renderer, handle_cache.renderer.onDrawFrame, NULL); // FIXME passing NULL only works if the app doesn't use this parameter
	if((*env)->ExceptionCheck(env))
		(*env)->ExceptionDescribe(env);
/* ... */
	glDebugMessageInsert(GL_DEBUG_SOURCE_THIRD_PARTY, GL_DEBUG_TYPE_MARKER, 0xdeadbeef, GL_DEBUG_SEVERITY_NOTIFICATION, strlen("---- returned from calling onDrawFrame"), "---- returned from calling onDrawFrame");
	printf("\n---- returned from calling onDrawFrame\n\n\n\n");

	glFlush();
	//SUS
	glFinish();
	eglSwapBuffers(eglDisplay, eglSurface);

	printf("WEEEE AAAAARE HEEEEERE SIX\n");

	result = eglMakeCurrent(old_eglDisplay, old_read_surface, old_draw_surface, old_eglContext);
	printf("eglMakeCurrent for original (GTK's) context returned: %d\n", result);

	gtk_gl_area_make_current(area);
/* --- */

	printf("OpenGL version after restore: >%s<\n", glGetString(GL_VERSION));
	printf("\n\n\n---- drawing the texture containing our frame\n\n");

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(program);
	glBindVertexArray(vao);

	glActiveTexture(GL_TEXTURE0 + texUnit);
	glBindTexture(GL_TEXTURE_2D, texBufferdObject);
//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 8, 8, 0, GL_RGBA, GL_UNSIGNED_BYTE, &texture[0]);
    glEGLImageTargetTexture2DOES(GL_TEXTURE_2D, egl_image);
	glBindSampler(texUnit, sampler);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);

	printf("\n---- end of drawing the texture containing our frame\n\n\n\n");

	return TRUE;
}

struct jni_callback_data { JavaVM *jvm; jobject this; jclass this_class; };

static void call_ontouch_callback(int action, float x, float y, struct jni_callback_data *d)
{
	JNIEnv *env;
	(*d->jvm)->GetEnv(d->jvm, (void**)&env, JNI_VERSION_1_6);

	jobject motion_event = (*env)->NewObject(env, handle_cache.motion_event.class, handle_cache.motion_event.constructor, action, x, y);

	(*env)->CallBooleanMethod(env, d->this, handle_cache.gl_surface_view.onTouchEvent, motion_event);

	if((*env)->ExceptionCheck(env))
		(*env)->ExceptionDescribe(env);

	(*env)->DeleteLocalRef(env, motion_event);
}

static gboolean on_event(GtkEventControllerLegacy* self, GdkEvent* event, struct jni_callback_data *d)
{
	double x;
	double y;

	switch(gdk_event_get_event_type(event)) {
		case GDK_BUTTON_PRESS:
			gdk_event_get_position(event, &x, &y);
			call_ontouch_callback(MOTION_EVENT_ACTION_DOWN, (float)x, (float)y, d);
			break;
		case GDK_BUTTON_RELEASE:
			gdk_event_get_position(event, &x, &y);
			call_ontouch_callback(MOTION_EVENT_ACTION_UP, (float)x, (float)y, d);
			break;
		case GDK_MOTION_NOTIFY:
			call_ontouch_callback(MOTION_EVENT_ACTION_MOVE, (float)x, (float)y, d);
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

	JavaVM *jvm;
	(*env)->GetJavaVM(env, &jvm);

	struct jni_gl_callback_data *gl_callback_data = malloc(sizeof(struct jni_gl_callback_data));
	gl_callback_data->jvm = jvm;
	gl_callback_data->renderer = _REF(renderer);
	gl_callback_data->first_time = 1;

	g_signal_connect(gl_area, "render", G_CALLBACK(render), gl_callback_data);

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
}
