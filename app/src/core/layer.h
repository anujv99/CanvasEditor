
/*
 *	A layer can be used outside the app to do stuff
 */

#ifndef __LAYER_H__
#define __LAYER_H__

#include <string>

#include <utils/handledobject.h>
#include <core/events/event.h>
#include <core/timer.h>

namespace app { namespace core {

	class Layer : public utils::HandledObject<Layer> {
		friend class utils::StrongHandle<Layer>;
	protected:
		Layer(const std::string & name) : m_Name(name) {}
		virtual ~Layer() {}
	public:
		virtual void OnUpdate(TimePoint dt) {}
		virtual void OnRender() {}
		virtual void OnImGuiUpdate() {}
		virtual void OnEvent(events::Event & e) {}

		inline std::string GetName() const { return m_Name; }
	private:
		std::string m_Name;
	};

} }

#endif //__LAYER_H__
