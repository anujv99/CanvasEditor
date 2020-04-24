
#ifndef __IMGUI_MANAGER_H__
#define __IMGUI_MANAGER_H__

#include <cstdint>

#include <imgui.h>
#include <utils/singleton.h>

namespace app { namespace imgui {

	class ImGuiManager : public utils::Singleton<ImGuiManager> {
		friend class utils::Singleton<ImGuiManager>;
	private:
		ImGuiManager();
		~ImGuiManager();
	public:
		void BeginFrame();
		void EndFrame();
	};

} }

#endif //__IMGUI_MANAGER_H__
