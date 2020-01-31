#include "imguiwindow.h"

namespace app { namespace imgui {

	ImGuiWindow::ImGuiWindow() :
		Pos(-1),
		Dimen(0),
		DimenAutoSize(0),
		DimenAutoSizePrev(0),
		ScrollPos(0),
		TitleBarMinWidth(0),
		BackgroundAlpha(0.5f),
		AutoSize(false),
		IsMinimized(false),
		IsNewWindow(true),
		IsLocked(false),
		FramesSinceUpdate(0)
	{}

} }