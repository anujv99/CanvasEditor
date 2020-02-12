#include "layerstack.h"

#include <algorithm>

namespace app { namespace core {

	LayerStack::LayerStack() {}

	LayerStack::~LayerStack() {}

	void LayerStack::PushLayer(utils::StrongHandle<Layer> layer) {
		ASSERTM(layer != nullptr, "Invalid layer");
		m_Layers.push_back(layer);
	}

	void LayerStack::PopLayer(utils::StrongHandle<Layer> layer) {
		ASSERTM(layer != nullptr, "Invalid layer");

		auto it = std::find(m_Layers.begin(), m_Layers.end(), layer);
		if (it == m_Layers.end()) {
			LOG_ERROR("Unable to find layer to pop : %s", layer->GetName());
			return;
		}

		m_Layers.erase(it);
	}

} }