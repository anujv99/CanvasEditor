
#include "application.h"

using namespace app;

int main() {
	
	Application::CreateInst();

	Application::Ref().Run();

	Application::Ref().DestroyInst();

}
