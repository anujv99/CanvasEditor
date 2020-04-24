#include "imguimanager.h"

#include "core/window.h"
#include "core/input.h"

#ifdef __OPENGL__
	#include <examples/imgui_impl_opengl3.h>
	#include <examples/imgui_impl_opengl3.cpp>
#else
	#error "only opengl supported"
#endif

#ifdef __GLFW__
	#include <examples/imgui_impl_glfw.h>
	#include <examples/imgui_impl_glfw.cpp>
#else
	#error "only glfw supported"
#endif

#include <GLFW/glfw3.h>

namespace app { namespace imgui {

	ImGuiManager::ImGuiManager() {
		IMGUI_CHECKVERSION();

		ImGui::CreateContext();
		ImGuiIO & io = ImGui::GetIO(); (void)io;
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		
		// Setup Dear ImGui style
		ImGui::StyleColorsDark();

		const char gl_version[] = "#version 450 core";

		ImGui_ImplGlfw_InitForOpenGL((GLFWwindow *)core::Window::Ref().GetRawWindow(), true);
		ImGui_ImplOpenGL3_Init(gl_version);
	}

	ImGuiManager::~ImGuiManager() {
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void ImGuiManager::BeginFrame() {
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGuiIO & io = ImGui::GetIO();
		if (io.WantCaptureMouse) {
			core::Input::ConsumeMouseClick(MOUSE_BUTTON_LEFT);
		}
	}

	void ImGuiManager::EndFrame() {
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

} }

#include <misc/cpp/imgui_stdlib.h>
#include <misc/cpp/imgui_stdlib.cpp>