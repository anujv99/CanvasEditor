
#include <application.h>
#include <renderer/renderer.h>

using namespace app;

class TestLayer : public core::Layer {
public:
	TestLayer() : Layer("Test Layer") {
		
	}

	virtual void OnRender() override {
	}

private:
};

int main() {

	Application::CreateInst();

	Application::Ref().PushLayer(new TestLayer());

	Application::Ref().Run();

	Application::Ref().DestroyInst();

}
