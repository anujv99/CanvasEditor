
/*
 *	Application class is responsible to initializing the submodules eg :- Window, GraphicsContext etc
 */

#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include <core/events/event.h>
#include <core/events/windowevent.h>
#include <core/layerstack.h>
#include <core/timer.h>

#include <utils/singleton.h>

namespace app {

	class Application : public utils::Singleton<Application> {
		friend class utils::Singleton<Application>;
	private:
		Application();
		~Application();
	public:
		void Run();

		inline void PushLayer(utils::StrongHandle<core::Layer> layer) { m_LayerStack.PushLayer(layer); }
		inline void PopLayer(utils::StrongHandle<core::Layer> layer) { m_LayerStack.PopLayer(layer); }
	private:
		void Update();
		void Render();
		void Gui();

		void OnEvent(core::events::Event & e);
		bool OnWindowClose(core::events::WindowCloseEvent & e);
	private:
		bool  m_IsRunning;
		core::LayerStack m_LayerStack;
	};

}

#endif //__APPLICATION_H__
