#include "application.h"

#include "core/window.h"
#include "core/graphicscontext.h"
#include "core/input.h"

#include "math/mvpstack.h"
#include "renderer/renderer.h"
#include "renderer/immgfx.h"

#include "imgui/imguimanager.h"

#include "vm/vm.h"

#include "utils/fileutils.h"

#include "graphics/renderstate.h"
#include "graphics/framebuffer.h"

namespace app {

	Application::Application() : m_IsRunning(true) {
		vm::VM::CreateInst();

		vm::WindowConfig config = vm::VM::Ref().ReadConfigFile(utils::FileUtils::ConvertToRelativePath("res/scripts/config.lua").c_str());

		core::Window::CreateInst();
		core::Window::Ref().Create(config.Width, config.Height, config.Title.c_str());

		core::Window::Ref().SetEventFunction(BIND_EVENT_FN(Application::OnEvent));

		core::GraphicsContext::CreateInst();
		core::GraphicsContext::Ref().CreateContext(core::Window::Ref().GetRawWindow());

		graphics::RenderState::CreateInst();

		math::MVPStack::CreateInst();

		renderer::Renderer::CreateInst();
		renderer::ImmGFX::CreateInst();

		imgui::ImGuiManager::CreateInst();

		graphics::BlendFunction func;
		func.SrcColor = graphics::BlendColorOption::SRC_ALPHA;
		func.DestColor = graphics::BlendColorOption::INV_SRC_ALPHA;
		func.ColorOperation = graphics::BlendOperation::ADD;

		func.SrcAlpha = graphics::BlendAlphaOption::ONE;
		func.DestAlpha = graphics::BlendAlphaOption::ONE;
		func.AlphaOperation = graphics::BlendOperation::ADD;
		graphics::RenderState::Ref().SetBlendFunction(func);

		Mat4 projection = Mat4::Ortho(0.0f, (float)core::Window::Ref().GetWidth(), 0.0f, (float)core::Window::Ref().GetHeight(), -1.0f, 1.0f);
		math::MVPStack::Ref().Projection().Push(projection);
	}

	Application::~Application() {
		math::MVPStack::Ref().Projection().Pop();

		imgui::ImGuiManager::DestroyInst();
		graphics::RenderState::DestroyInst();
		renderer::Renderer::DestroyInst();
		renderer::ImmGFX::DestroyInst();
		math::MVPStack::DestroyInst();
		core::GraphicsContext::DestroyInst();
		core::Window::DestroyInst();
		vm::VM::DestroyInst();
	}

	void Application::Run() {
		
		vm::VM::Ref().Initialize(utils::FileUtils::ConvertToRelativePath("res/scripts/main.lua").c_str());

		while (m_IsRunning) {
			core::Timer::Update();

			core::GraphicsContext::Ref().BeginFrame();

			graphics::RenderState::Ref().SetTopology(graphics::Topology::TRIANGLE);

			imgui::ImGuiManager::Ref().BeginFrame();

			Update();

			Render();

			Gui();

			imgui::ImGuiManager::Ref().EndFrame();

			core::GraphicsContext::Ref().EndFrame();
			core::Input::Update();
			core::Window::Ref().Update();
		}

	}

	void Application::Update() {
		vm::VM::Ref().Update(core::Timer::GetDeltaTime().GetS());
		for (auto & l : m_LayerStack) {
			l->OnUpdate(core::Timer::GetDeltaTime());
		}
	}

	void Application::Render() {

		renderer::Renderer::Ref().BeginScene();

		vm::VM::Ref().Render();

		{
			for (auto & l : m_LayerStack) {
				l->OnRender();
			}

			renderer::ImmGFX::Ref().Render();
		}

		renderer::Renderer::Ref().EndScene();
	}

	void Application::Gui() {
		for (auto & l : m_LayerStack) {
			l->OnImGuiUpdate();
		}

		vm::VM::Ref().Gui();
	}

	void Application::OnEvent(core::events::Event & e) {
		core::Input::OnEvent(e);

		core::events::EventDispatcher dispatcher(e);
		 
		dispatcher.Dispatch<core::events::WindowCloseEvent>(BIND_EVENT_FN(Application::OnWindowClose));
		dispatcher.Dispatch<core::events::WindowResizeEvent>(BIND_EVENT_FN(Application::OnWindowResize));

		for (auto & l : m_LayerStack) {
			l->OnEvent(e);
		}
	}
	 
	bool Application::OnWindowClose(core::events::WindowCloseEvent & e) {
		m_IsRunning = false;
		return true;
	}

	bool Application::OnWindowResize(core::events::WindowResizeEvent & e) {
		
		Mat4 projection = Mat4::Ortho(0.0f, (float)e.GetWindowSizeX(), 0.0f, (float)e.GetWindowSizeY(), -1.0f, 1.0f);

		math::MVPStack::Ref().Projection().Pop();
		math::MVPStack::Ref().Projection().Push(projection);

		graphics::Viewport vp;
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;
		vp.Width = e.GetWindowSizeX();
		vp.Height = e.GetWindowSizeY();

		graphics::RenderState::Ref().SetViewport(vp);

		return false;
	}

}
