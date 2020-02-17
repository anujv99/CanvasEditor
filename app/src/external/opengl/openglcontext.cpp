#include "openglcontext.h"

#include <utils/assert.h>

#include <glad/glad.h>

namespace app {

	static void GLAPIENTRY DebugCallback(
		GLenum source,
		GLenum type,
		GLuint id,
		GLenum severity,
		GLsizei length,
		const GLchar * message,
		const void * userParam) {

		static const char * sev = nullptr;
		switch (severity) {
		case GL_DEBUG_SEVERITY_HIGH:
			sev = "SEVERITY_HIGH";
			break;
		case GL_DEBUG_SEVERITY_MEDIUM:
			sev = "SEVERITY_MEDIUM";
			break;
		case GL_DEBUG_SEVERITY_LOW:
			sev = "SEVERITY_LOW";
			break;
		case GL_DEBUG_SEVERITY_NOTIFICATION:
			sev = "SEVERITY_NOTIFICATION";
			return;
		default:
			break;
		}

		static const char * tpe = nullptr;
		switch (type) {
		case GL_DEBUG_TYPE_ERROR:
			tpe = "DEBUG_TYPE_ERROR";
			break;
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
			tpe = "DEBUG_TYPE_DEPRECATED_BEHAVIOR";
			break;
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
			tpe = "DEBUG_TYPE_UNDEFINED_BEHAVIOR";
			break;
		case GL_DEBUG_TYPE_PORTABILITY:
			tpe = "DEBUG_TYPE_PORTABILITY";
			break;
		case GL_DEBUG_TYPE_PERFORMANCE:
			tpe = "DEBUG_TYPE_PERFORMANCE";
			break;
		case GL_DEBUG_TYPE_MARKER:
			tpe = "DEBUG_TYPE_MARKER";
			break;
		case GL_DEBUG_TYPE_PUSH_GROUP:
			tpe = "DEBUG_TYPE_PUSH_GROUP";
			break;
		case GL_DEBUG_TYPE_POP_GROUP:
			tpe = "DEBUG_TYPE_POP_GROUP";
			break;
		case GL_DEBUG_TYPE_OTHER:
			tpe = "DEBUG_TYPE_OTHER";
			break;
		default:
			break;
		}

		switch (severity) {
		case GL_DEBUG_SEVERITY_HIGH:
		{
			LOG_ERROR("[OpenGL] GL CALLBACK: %s\ntype = %s\nseverity = %s\nmessage = %s\n",
				type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "",
				tpe, sev, message);
			break;
		}
		case GL_DEBUG_SEVERITY_MEDIUM:
		case GL_DEBUG_SEVERITY_LOW:
		{
			LOG_WARN("[OpenGL] GL CALLBACK: %s\ntype = %s\nseverity = %s\nmessage = %s\n",
				type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "",
				tpe, sev, message);
			break;
		}
		case GL_DEBUG_SEVERITY_NOTIFICATION:
		{
			LOG_INFO("[OpenGL] GL CALLBACK: %s\ntype = %s\nseverity = %s\nmessage = %s\n",
				type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "",
				tpe, sev, message);
			break;
		}
		default:
			break;
		}

	}

	namespace core {
		GraphicsContext * GraphicsContext::CreateContext() {
			return dynamic_cast<GraphicsContext *>(new app::external::opengl::OpenGLContext());
		}
	}

	namespace external { namespace opengl {

		OpenGLContext::OpenGLContext() : m_Window(nullptr) {}

		OpenGLContext::~OpenGLContext() {}

		void OpenGLContext::CreateContext(uintptr_t rawWindowPtr) {
			m_Window = (::GLFWwindow *)rawWindowPtr;
			ASSERTM(m_Window != nullptr, "[OpenGL] Invalid window");

			glfwMakeContextCurrent(m_Window);
			int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
			ASSERTM(status, "[OpenGL] Failed to create OpenGL context");

			#ifdef __DEBUG__
				glEnable(GL_DEBUG_OUTPUT);
				glDebugMessageCallback(DebugCallback, nullptr);
			#endif

			LOG_INFO("[OpenGL] Successfully created OpenGL context");

			LOG_INFO("OpenGL Info:");
			LOG_INFO("\tVendor: %s", glGetString(GL_VENDOR));
			LOG_INFO("\tRenderer: %s", glGetString(GL_RENDERER));
			LOG_INFO("\tVersion: %s", glGetString(GL_VERSION));

			GLint versionMajor, versionMinor;
			glGetIntegerv(GL_MAJOR_VERSION, &versionMajor);
			glGetIntegerv(GL_MINOR_VERSION, &versionMinor);

			ASSERTM(versionMajor > 4 || (versionMajor == 4 && versionMinor >= 5), "[OpenGL] Cannot run software below OpenGL version 4.5");

			glDisable(GL_DEPTH_TEST);
			glfwSwapInterval(0);
		}

		void OpenGLContext::BeginFrame() {
			glClearColor(0.8f, 0.4f, 0.2f, 0.0f);
			glClear(GL_COLOR_BUFFER_BIT);
		}

		void OpenGLContext::EndFrame() {
			glfwSwapBuffers(m_Window);
		}

	} }

}
