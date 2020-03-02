#include "application.h"

#include "core/window.h"
#include "core/graphicscontext.h"
#include "core/input.h"

#include "math/mvpstack.h"
#include "renderer/renderer.h"
#include "renderer/immgfx.h"

#include "imgui/imguimanager.h"
#include "imgui/imgui.h"

#include "vm/vm.h"

#include "utils/fileutils.h"

#include "graphics/renderstate.h"
#include "graphics/framebuffer.h"

namespace app {

	static utils::StrongHandle<graphics::Framebuffer> fbo;

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

		Mat4 projection = Mat4::Ortho(0.0f, core::Window::Ref().GetWidth(), core::Window::Ref().GetHeight(), 0.0f, -1.0f, 1.0f);
		math::MVPStack::Ref().Projection().Push(projection);

		fbo = graphics::Framebuffer::Create(
			Vec2i(core::Window::Ref().GetWidth(), core::Window::Ref().GetHeight()),
			graphics::TextureFormat::RGBA, graphics::FBOFlags::MSAA_16X
		);

		graphics::RenderState::Ref().SetLineThickness(2.0f);
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

			imgui::ImGuiManager::Ref().DetectConsumeInputs();
			imgui::ImGuiManager::Ref().PreUpdate();

			graphics::RenderState::Ref().SetTopology(graphics::Topology::TRIANGLE);

			Update();

			Render();

			Gui();

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
		fbo->Bind();
		fbo->Clear();

		vm::VM::Ref().Render();

		{
			for (auto & l : m_LayerStack) {
				l->OnRender();
			}
			renderer::ImmGFX::Ref().Render();
		}

		fbo->UnBind();
		fbo->Resolve();
		renderer::Renderer::Ref().PassFramebuffer(fbo, nullptr);
	}

	void Application::Gui() {
		for (auto & l : m_LayerStack) {
			l->OnImGuiUpdate();
		}

		vm::VM::Ref().Gui();

		Mat4 ortho = Mat4::Ortho(0, core::Window::Ref().GetWidth(), 0, core::Window::Ref().GetHeight(), -1, 1);
		math::MVPStack::Ref().Projection().Push(ortho);

		renderer::Renderer::Ref().BeginScene();

		imgui::ImGuiManager::Ref().PostUpdate();

		math::MVPStack::Ref().Projection().Pop();

		renderer::Renderer::Ref().EndScene();
	}

	void Application::OnEvent(core::events::Event & e) {
		core::Input::OnEvent(e);

		core::events::EventDispatcher dispatcher(e);

		dispatcher.Dispatch<core::events::WindowCloseEvent>(BIND_EVENT_FN(Application::OnWindowClose));

		for (auto & l : m_LayerStack) {
			l->OnEvent(e);
		}
	}

	bool Application::OnWindowClose(core::events::WindowCloseEvent & e) {
		m_IsRunning = false;
		return true;
	}

}
