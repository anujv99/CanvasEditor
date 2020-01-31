#include "application.h"

#include "core/window.h"
#include "core/graphicscontext.h"
#include "core/input.h"

#include "math/mvpstack.h"
#include "renderer/renderer.h"

#include "imgui/imguimanager.h"
#include "imgui/imgui.h"

#include "graphics/framebuffer.h"

namespace app {

	utils::StrongHandle<graphics::Framebuffer> fbo;

	Application::Application() : m_IsRunning(true) {
		core::Window::CreateInst();
		core::Window::Ref().Create(1280, 720, "App Window");

		core::Window::Ref().SetEventFunction(BIND_EVENT_FN(Application::OnEvent));

		core::GraphicsContext::CreateInst();
		core::GraphicsContext::Ref().CreateContext(core::Window::Ref().GetRawWindow());

		graphics::RenderState::CreateInst();

		math::MVPStack::CreateInst();

		renderer::Renderer::CreateInst();

		imgui::ImGuiManager::CreateInst();

		graphics::BlendFunction func;
		func.SrcColor = graphics::BlendColorOption::SRC_ALPHA;
		func.DestColor = graphics::BlendColorOption::INV_SRC_ALPHA;
		func.ColorOperation = graphics::BlendOperation::ADD;

		func.SrcAlpha = graphics::BlendAlphaOption::ONE;
		func.DestAlpha = graphics::BlendAlphaOption::ONE;
		func.AlphaOperation = graphics::BlendOperation::ADD;
		graphics::RenderState::Ref().SetBlendFunction(func);
	
		fbo = graphics::Framebuffer::Create(Vec2i(1280, 720), graphics::TextureFormat::RGBA);
		//fbo->GetTexture()->Bind(0);
	}

	Application::~Application() {
		imgui::ImGuiManager::DestroyInst();
		graphics::RenderState::DestroyInst();
		renderer::Renderer::DestroyInst();
		math::MVPStack::DestroyInst();
		core::GraphicsContext::DestroyInst();
		core::Window::DestroyInst();
	}

	void Application::Run() {
		
		while (m_IsRunning) {
			core::GraphicsContext::Ref().BeginFrame();

			imgui::ImGuiManager::Ref().DetectConsumeInputs();
			imgui::ImGuiManager::Ref().PreUpdate();

			Render();

			Gui();
			
			core::GraphicsContext::Ref().EndFrame();
			core::Input::Update();
			core::Window::Ref().Update();
		}

	}

	void Application::Render() {
		fbo->Bind();
		fbo->Clear(Vec4(0.0f, 0.0f, 0.0f, 1.0f));
		
		renderer::Renderer::Ref().BeginScene();
		
		renderer::Renderer::Ref().DrawSprite(Vec2(0.0f), Vec2(1.0f), Vec4(1.0f));
		
		renderer::Renderer::Ref().EndScene();
		
		ImGui::Begin("Hello");
		ImGui::End();
		
		Mat4 proj = Mat4::Ortho(0, 1280, 0, 720, -1, 1);
		math::MVPStack::Ref().Projection().Push(proj);
		renderer::Renderer::Ref().BeginScene();

		imgui::ImGuiManager::Ref().PostUpdate();

		renderer::Renderer::Ref().EndScene();
		math::MVPStack::Ref().Projection().Pop();

		fbo->UnBind();

		renderer::Renderer::Ref().BeginScene();

		renderer::Renderer::Ref().DrawSprite(Vec2(0.0f), Vec2(1.0f), Vec4(1.0f), Vec2(0.0f, 1.0f), Vec2(1.0f, 0.0f), fbo->GetTexture());

		renderer::Renderer::Ref().EndScene();

	}

	void Application::Gui() {

	}

	void Application::OnEvent(core::events::Event & e) {
		core::Input::OnEvent(e);

		core::events::EventDispatcher dispatcher(e);

		dispatcher.Dispatch<core::events::WindowCloseEvent>(BIND_EVENT_FN(Application::OnWindowClose));
	}

	bool Application::OnWindowClose(core::events::WindowCloseEvent & e) {
		m_IsRunning = false;
		return true;
	}

}
