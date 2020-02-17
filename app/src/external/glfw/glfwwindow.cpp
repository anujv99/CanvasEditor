#include "glfwwindow.h"

#include <utils/assert.h>

#include <core/events/event.h>
#include <core/events/keyevent.h>
#include <core/events/mouseevent.h>
#include <core/events/windowevent.h>

namespace app {

	namespace core {
		Window * Window::CreateWindow() {
			return dynamic_cast<Window *>(new external::glfw::GLFWWindow());
		}
	}

	namespace external { namespace glfw {

		GLFWWindow::GLFWWindow() : m_IsCreated(false), m_Data{ 0u, 0u, "", nullptr }, m_Window(nullptr) {
			m_Data.EventFunc = [](app::core::events::Event &) -> void {};	//Assigning to reduce checking for every event
		}

		GLFWWindow::~GLFWWindow() {
			if (m_IsCreated) {
				glfwDestroyWindow(m_Window);
			}
			glfwTerminate();
		}

		void GLFWWindow::Create(unsigned int width, unsigned int height, const char * title) {
			ASSERTM(glfwInit(), "[GLFW] Failed to initialize GLFW");

			::GLFWwindow * window = glfwCreateWindow(width, height, title, nullptr, nullptr);

			ASSERTM(window != nullptr, "[GLFW] Failed to create GLFW window");

			m_Data.Width	= width;
			m_Data.Height	= height;
			m_Data.Title	= title;

			m_Window		= window;
			m_IsCreated		= true;

			LOG_INFO("[GLFW] Successfully created GLFW window");

			glfwSetWindowUserPointer(m_Window, (void *)&m_Data);

			if (glfwRawMouseMotionSupported()) {
				glfwSetInputMode(m_Window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
			}

			SetCallbacks();
		}

		void GLFWWindow::Update() {
			glfwWaitEvents();
		}

		uintptr_t GLFWWindow::GetRawWindow() {
			return (uintptr_t)m_Window;
		}

		void GLFWWindow::SetEventFunction(std::function<void(app::core::events::Event &)> eventFunc) {
			if (eventFunc) {
				m_Data.EventFunc = eventFunc;
			}
		}

		void GLFWWindow::SetCallbacks() {
			glfwSetWindowCloseCallback(m_Window, [](::GLFWwindow * window) -> void {
				WindowData * data = (WindowData *)glfwGetWindowUserPointer(window);
				app::core::events::WindowCloseEvent e;
				data->EventFunc(e);
			});

			glfwSetWindowSizeCallback(m_Window, [](::GLFWwindow * window, int width, int height) -> void {
				WindowData * data = (WindowData *)glfwGetWindowUserPointer(window);
				app::core::events::WindowResizeEvent e(width, height);
				data->EventFunc(e);
				data->Width = e.GetWindowSizeX();
				data->Height = e.GetWindowSizeY();
			});

			glfwSetWindowPosCallback(m_Window, [](::GLFWwindow * window, int x, int y) -> void {
				WindowData * data = (WindowData *)glfwGetWindowUserPointer(window);
				app::core::events::WindowMoveEvent e(x, y);
				data->EventFunc(e);
			});

			glfwSetKeyCallback(m_Window, [](::GLFWwindow * window, int keyCode, int scanCode, int action, int mods) -> void {
				WindowData * data = (WindowData *)glfwGetWindowUserPointer(window);
				switch (action) {
				case GLFW_PRESS:
				{
					app::core::events::KeyPressedEvent e(keyCode, false);
					data->EventFunc(e);

					break;
				}
				case GLFW_REPEAT:
				{
					app::core::events::KeyPressedEvent e(keyCode, true);
					data->EventFunc(e);
					break;
				}
				case GLFW_RELEASE:
				{
					app::core::events::KeyReleasedEvent e(keyCode);
					data->EventFunc(e);
					break;
				}
				}
			});

			glfwSetMouseButtonCallback(m_Window, [](::GLFWwindow * window, int button, int action, int mods) -> void {
				WindowData * data = (WindowData *)glfwGetWindowUserPointer(window);
				switch (action) {
				case GLFW_PRESS:
				{
					app::core::events::MouseButtonPressedEvent e(button);
					data->EventFunc(e);
					break;
				}
				case GLFW_RELEASE:
				{
					app::core::events::MouseButtonReleasedEvent e(button);
					data->EventFunc(e);
					break;
				}
				}
			});

			glfwSetCursorPosCallback(m_Window, [](::GLFWwindow * window, double x, double y) -> void {
				WindowData * data = (WindowData *)glfwGetWindowUserPointer(window);
				app::core::events::MouseMovedEvent e(x, y);
				data->EventFunc(e);
			});

			glfwSetScrollCallback(m_Window, [](::GLFWwindow * window, double xOffset, double yOffset) -> void {
				WindowData * data = (WindowData *)glfwGetWindowUserPointer(window);
				app::core::events::MouseMovedEvent e(xOffset, yOffset);
				data->EventFunc(e);
			});

			glfwSetCharCallback(m_Window, [](::GLFWwindow * window, unsigned int character) -> void {
				WindowData * data = (WindowData *)glfwGetWindowUserPointer(window);  
				app::core::events::CharacterEvent e(character);
				data->EventFunc(e);
			});
		}

	} }

}
