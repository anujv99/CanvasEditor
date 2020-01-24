#include "openglcontext.h"

#include <utils/assert.h>

#include <glad/glad.h>

namespace app {

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

			LOG_INFO("[OpenGL] Successfully created OpenGL context");

			LOG_INFO("OpenGL Info:");
			LOG_INFO("\tVendor: %s", glGetString(GL_VENDOR));
			LOG_INFO("\tRenderer: %s", glGetString(GL_RENDERER));
			LOG_INFO("\tVersion: %s", glGetString(GL_VERSION));

			GLint versionMajor, versionMinor;
			glGetIntegerv(GL_MAJOR_VERSION, &versionMajor);
			glGetIntegerv(GL_MINOR_VERSION, &versionMinor);

			ASSERTM(versionMajor > 4 || (versionMajor == 4 && versionMinor >= 5), "Cannot run software below OpenGL version 4.5");

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
