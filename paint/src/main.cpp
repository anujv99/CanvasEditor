
#include <application.h>
#include <imgui.h>
#include <renderer/renderer.h>
#include <network/networkvm.h>
#include <vm/vm.h>

#include "paintrenderer.h"

using namespace app;

#include <iostream>

class TestLayer : public core::Layer {
public:
	TestLayer() : Layer("Network Layer") {

	}
	~TestLayer() {
	}
	void OnImGuiUpdate() override {
		paint::NetworkVM::Ref().Gui();
	}
private:
	std::string m_Msg;
};

int main() {
	paint::NetworkVM::CreateInst();
	Application::CreateInst();
	paint::PaintRenderer::CreateInst();

	paint::NetworkVM::Ref().LuaBindNetworkLib(vm::VM::Ref().GetState());
	paint::PaintRenderer::Ref().BindLuaLib(vm::VM::Ref().GetState());
	
	Application::Ref().PushLayer(new TestLayer());
	
	Application::Ref().Run();
	
	paint::PaintRenderer::DestroyInst();
	Application::Ref().DestroyInst();
	paint::NetworkVM::DestroyInst();
}
