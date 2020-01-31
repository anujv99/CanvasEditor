
#ifndef __IMGUI_MANAGER_H__
#define __IMGUI_MANAGER_H__

#include <unordered_map>
#include <vector>
#include <set>
#include <string>

#include <utils/handledobject.h>
#include <utils/stronghandle.h>
#include <utils/singleton.h>
#include <renderer/font.h>
#include <math/math.h>

#include "imguiwindow.h"

namespace app { namespace imgui {

	class ImGuiManager : public utils::Singleton<ImGuiManager> {
		friend class utils::Singleton<ImGuiManager>;
	private:
		ImGuiManager();
		~ImGuiManager();
	public:
		struct ImGuiStateStruct {
			union {
				float				F;
				int					I;
				unsigned int		UInt;
				struct { float		FVec[4]; };
				struct { int		IVec[4]; };
			} Data;

			Vec2i DrawPos;
			Vec2i DrawPosPrev;
			Vec2i ScrollOffset;

			int NumTabs;

			Vec2i MouseRegionStart;
			Vec2i MouseRegionDimen;

			static constexpr const int WIDGET_NULL		= -1;
			static constexpr const int WIDGET_WINDOW	=  1;

			int ActiveWidgetID;
			int WidgetCount;
			bool DoesWindowConsumeTextInput;

			utils::StrongHandle<ImGuiWindow> ActiveWindow;
			utils::StrongHandle<ImGuiWindow> WorkingWindow;

			void Reset();
		} State;

		void PreUpdate();
		void PostUpdate();
		void ClearActiveWindow();
		void ClearAllWindow();
		void OnResizeScreen(Vec2 prevDimen, Vec2 newDimen);

		void SetActiveWindow(utils::StrongHandle<ImGuiWindow> window);
		void SetActiveWidgetID(int id);
		void BringWindowToFront(utils::StrongHandle<ImGuiWindow> window);

		int GetWindowOrderIndex(const std::string & name) const;
		bool IsPosInWindow(Vec2i pos, utils::StrongHandle<ImGuiWindow> window) const;

		void DetectConsumeInputs();
		bool DidMouseJustGoDown(int button) const;
		bool DidKeyJustGoDown(int keyCode) const;

		utils::StrongHandle<ImGuiWindow> GetWindow(const std::string & name);

		inline utils::StrongHandle<renderer::Font> GetFont() { return m_Font; }
		inline std::unordered_map<unsigned int, utils::StrongHandle<ImGuiWindow>> & GetWindowList() { return m_WindowMap; }
	private:
		void PruneVisibleWindow();
		void UpdateWindowMap();
		void DebugPrintVisibleWindow();
		void DrawWindows();
	private:
		std::unordered_map<unsigned int, utils::StrongHandle<ImGuiWindow>> m_WindowMap;
		std::vector<utils::StrongHandle<ImGuiWindow>> m_VisibleWindows;
		// [TODO] Set window draw calls and setup rendering

		utils::StrongHandle<renderer::Font> m_Font;
		float m_FontScale;

		std::set<int> m_ConsumeMouseButtons;
		std::set<char> m_ConsumeKeyPress;
	public:
		//friend void ::app::ImGuiColor(Vec3, float);
		//friend void ::app::ImGuiDrawRect(Vec2i, Vec2i);
		//friend void ::app::ImGuiPrint(const std::string &, Vec2i);
		//friend void ::app::ImGuiDrawLine(Vec2i, Vec2i);
		struct WindowDrawCall : public utils::HandledObject<WindowDrawCall> {
			std::vector<Vec2> QuadPos;
			std::vector<Vec2> QuadDimen;
			std::vector<Vec4> QuadColor;

			std::vector<std::string> Text;
			std::vector<Vec2> TextPos;
			std::vector<Vec4> TextColor;
		};
		std::unordered_map<uint32_t, utils::StrongHandle<WindowDrawCall>> m_WindowDrawCalls;
		utils::StrongHandle<WindowDrawCall> GetDrawCall(utils::StrongHandle<ImGuiWindow> window);

		Vec4 m_CurrentColor;
	};

} }

#endif //__IMGUI_MANAGER_H__