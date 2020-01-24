
/*
 *	Application class is responsible to initializing the submodules eg :- Window, GraphicsContext etc
 */

#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include <core/events/event.h>
#include <core/events/windowevent.h>

#include <utils/singleton.h>

namespace app {

	class Application : public utils::Singleton<Application> {
		friend class utils::Singleton<Application>;
	private:
		Application();
		~Application();
	public:
		void Run();
	private:
		void OnEvent(core::events::Event & e);

		bool OnWindowClose(core::events::WindowCloseEvent & e);
	private:
		bool  m_IsRunning;
	};

}

#endif //__APPLICATION_H__
