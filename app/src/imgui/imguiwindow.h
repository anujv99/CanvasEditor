
#ifndef __IMGUI_WINDOW_H__
#define __IMGUI_WINDOW_H__

#include <math/math.h>
#include <utils/handledobject.h>

#include <string>

namespace app { namespace imgui {

	struct ImGuiWindow : public utils::HandledObject<ImGuiWindow> {
		std::string Name;				//	Name

		Vec2i Pos;						//	Center Position
		Vec2i Dimen;					//	Dimension of window
		Vec2i DimenAutoSize;			//	Autosize
		Vec2i DimenAutoSizePrev;		//	Autosize of last frame
		Vec2i ScrollPos;				//	Scroll position
		
		pvint TitleBarMinWidth;			//	Minimum title bar width;
		pvfloat BackgroundAlpha;		//	Background alpha

		bool AutoSize;					//	Autosize on/off
		bool IsMinimized;				//	Is minimized
		bool IsNewWindow;				//	True for new window
		bool IsLocked;					//	Lock window on/off

		pvint FramesSinceUpdate;		//	>=1 means window is not active

		ImGuiWindow();
	};

} }

#endif //__IMGUI_WINDOW_H__