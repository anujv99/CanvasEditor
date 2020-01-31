
#ifndef __IMGUI_H__
#define __IMGUI_H__

#include <string>

#include <math/math.h>
#include <utils/stronghandle.h>

namespace app {

	struct ImGui {
		static const pvint AUTOPOS  = -1;
		static const pvint AUTOSIZE = -1;

		static void Begin(const std::string & name, Vec2i pos = Vec2i(AUTOPOS), Vec2i dimen = Vec2i(AUTOSIZE));
		static void End();

		static void SameLine();
		static void Print(const std::string & str);
		static void Seperator();

		// Widgets
		static pvfloat SliderFloat(const std::string & name, pvfloat & val, pvfloat min, pvfloat max);
		static pvint SliderInt(const std::string & name, pvint & val, pvint min, pvint max);
		static Vec2 SliderVec2(const std::string & name, Vec2 & val, Vec2 min, Vec2 max);

		static Vec3 SliderRGB(const std::string & name, Vec3 & val);

		static int TextInput(const std::string & name, std::string & val, pvint width);

		static void PropertyEditor();

		static pvint FONT_WIDTH;
		static pvint FONT_HEIGHT;
		static pvint TITLE_BAR_HEIGHT;

		static void MoveDrawPosBy(Vec2i dimen);
		static void MoveDrawPosNextLine(Vec2i dimen);
	};

}

#endif //__IMGUI_H__