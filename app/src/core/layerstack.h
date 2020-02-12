
/*
 *	Layer stack is a simple list which contains all the layers
 *	Layer stack is used by the app to control the updating of all the layers
 */

#ifndef __LAYER_STACK_H__
#define __LAYER_STACK_H__

#include <list>

#include <utils/handledobject.h>

#include "layer.h"

namespace app { namespace core {

	class LayerStack : public utils::HandledObject<LayerStack> {
	public:
		LayerStack();
		~LayerStack();
	public:
		void PushLayer(utils::StrongHandle<Layer> layer);
		void PopLayer(utils::StrongHandle<Layer> layer);

		std::list<utils::StrongHandle<Layer>>::iterator begin() { return m_Layers.begin(); }
		std::list<utils::StrongHandle<Layer>>::iterator end() { return m_Layers.end(); }
		std::list<utils::StrongHandle<Layer>>::reverse_iterator rbegin() { return m_Layers.rbegin(); }
		std::list<utils::StrongHandle<Layer>>::reverse_iterator rend() { return m_Layers.rend(); }

		std::list<utils::StrongHandle<Layer>>::const_iterator begin() const { return m_Layers.begin(); }
		std::list<utils::StrongHandle<Layer>>::const_iterator end() const { return m_Layers.end(); }
		std::list<utils::StrongHandle<Layer>>::const_reverse_iterator rbegin() const { return m_Layers.rbegin(); }
		std::list<utils::StrongHandle<Layer>>::const_reverse_iterator rend() const { return m_Layers.rend(); }

	private:
		std::list<utils::StrongHandle<Layer>> m_Layers;
	};

} }

#endif //__LAYER_STACK_H__
