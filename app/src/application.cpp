#include "application.h"

#include "core/window.h"
#include "core/graphicscontext.h"

namespace app {

	Application::Application() : m_IsRunning(true) {
		core::Window::CreateInst();
		core::Window::Ref().Create(1280, 720, "App Window");

		core::Window::Ref().SetEventFunction(BIND_EVENT_FN(Application::OnEvent));

		core::GraphicsContext::CreateInst();
		core::GraphicsContext::Ref().CreateContext(core::Window::Ref().GetRawWindow());

	}

	Application::~Application() {
		core::GraphicsContext::DestroyInst();
		core::Window::DestroyInst();
	}

	void Application::Run() {
		
		while (m_IsRunning) {
			core::GraphicsContext::Ref().BeginFrame();

			core::GraphicsContext::Ref().EndFrame();
			
			core::Window::Ref().Update();
		}

	}

	void Application::OnEvent(core::events::Event & e) {
		core::events::EventDispatcher dispatcher(e);

		dispatcher.Dispatch<core::events::WindowCloseEvent>(BIND_EVENT_FN(Application::OnWindowClose));
	}

	bool Application::OnWindowClose(core::events::WindowCloseEvent & e) {
		m_IsRunning = false;
		return true;
	}

}