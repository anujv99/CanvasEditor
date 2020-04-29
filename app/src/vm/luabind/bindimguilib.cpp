#include "bindimguilib.h"

#include "luabind.h"

#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>
#include <math/math.h>

namespace app {

	struct LuaImGuiFunc {
		static int Begin(lua_State * L) {
			LUA_CHECK_NUM_PARAMS(1);
			LUA_STRING_PARAM(1, name);
			ImGui::Begin(name, (bool *)0, ImGuiWindowFlags_AlwaysAutoResize);
			return 0;
		}

		static int TextInput(lua_State * L) {
			LUA_CHECK_NUM_PARAMS(2);
			LUA_STRING_PARAM(1, name);
			LUA_STRING_PARAM(2, text);

			std::string str(text);

			ImGui::InputText(name, &str);

			lua_pushstring(L, str.c_str());
			return 1;
		}

		static int End(lua_State * L) {
			LUA_CHECK_NUM_PARAMS(0);
			ImGui::End();
			return 0;
		}

		static int SameLine(lua_State * L) {
			LUA_CHECK_NUM_PARAMS(0);
			ImGui::SameLine();
			return 0;
		}

		static int SliderFloat(lua_State * L) {
			LUA_CHECK_NUM_PARAMS(4);
			LUA_STRING_PARAM(1, name);
			LUA_FLOAT_PARAM(2, val);
			LUA_FLOAT_PARAM(3, min);
			LUA_FLOAT_PARAM(4, max);

			ImGui::SliderFloat(name, &val, min, max);

			lua_pushnumber(L, val);
			return 1;
		}

		static int SliderInt(lua_State * L) {
			LUA_CHECK_NUM_PARAMS(4);
			LUA_STRING_PARAM(1, name);
			LUA_INT_PARAM(2, val);
			LUA_INT_PARAM(3, min);
			LUA_INT_PARAM(4, max);

			ImGui::SliderInt(name, &val, min, max);

			lua_pushnumber(L, (lua_Number)val);
			return 1;
		}

		static int SliderVec2(lua_State * L) {
			LUA_CHECK_NUM_PARAMS(4);
			LUA_STRING_PARAM(1, name);
			LUA_VEC2_PARAM(2, val);
			LUA_FLOAT_PARAM(3, min);
			LUA_FLOAT_PARAM(4, max);

			ImGui::SliderFloat2(name, &val->x, min, max);

			return 0;
		}

		static int SliderVec3(lua_State * L) {
			LUA_CHECK_NUM_PARAMS(4);
			LUA_STRING_PARAM(1, name);
			LUA_VEC3_PARAM(2, val);
			LUA_FLOAT_PARAM(3, min);
			LUA_FLOAT_PARAM(4, max);

			ImGui::SliderFloat3(name, &val->x, min, max);

			return 0;
		}

		static int SliderVec4(lua_State * L) {
			LUA_CHECK_NUM_PARAMS(4);
			LUA_STRING_PARAM(1, name);
			LUA_VEC4_PARAM(2, val);
			LUA_FLOAT_PARAM(3, min);
			LUA_FLOAT_PARAM(4, max);

			ImGui::SliderFloat4(name, &val->x, min, max);

			return 0;
		}

		static int SliderRGB(lua_State * L) {
			LUA_CHECK_NUM_PARAMS(2);
			LUA_STRING_PARAM(1, name);
			LUA_VEC3_PARAM(2, val);

			ImGui::ColorEdit3(name, &val->x);

			return 0;
		}

		static int SliderRGBA(lua_State * L) {
			LUA_CHECK_NUM_PARAMS(2);
			LUA_STRING_PARAM(1, name);
			LUA_VEC4_PARAM(2, val);

			ImGui::ColorEdit4(name, &val->x);

			return 0;
		}

		static int Button(lua_State * L) {
			LUA_CHECK_NUM_PARAMS(1);
			LUA_STRING_PARAM(1, name);
			lua_pushboolean(L, (int)ImGui::Button(name));
			return 1;
		}

		static int Checkbox(lua_State * L) {
			LUA_CHECK_NUM_PARAMS(2);
			LUA_STRING_PARAM(1, name);
			LUA_BOOL_PARAM(2, selected);
			lua_pushboolean(L, (int)ImGui::Checkbox(name, &selected));
			return 1;
		}

		static int RadioButton(lua_State * L) {
			LUA_CHECK_NUM_PARAMS(2);
			LUA_STRING_PARAM(1, name);
			LUA_BOOL_PARAM(2, selected);
			lua_pushboolean(L, (int)ImGui::RadioButton(name, selected));
			return 1;
		}
	};

	LUA_LIB_START(ImGui)
		LUA_LIB_ENTRY("Begin", LuaImGuiFunc::Begin)
		LUA_LIB_ENTRY("End", LuaImGuiFunc::End)
		LUA_LIB_ENTRY("SameLine", LuaImGuiFunc::SameLine)
		LUA_LIB_ENTRY("TextInput", LuaImGuiFunc::TextInput)
		LUA_LIB_ENTRY("SliderFloat", LuaImGuiFunc::SliderFloat)
		LUA_LIB_ENTRY("SliderInt", LuaImGuiFunc::SliderInt)
		LUA_LIB_ENTRY("SliderVec2", LuaImGuiFunc::SliderVec2)
		LUA_LIB_ENTRY("SliderVec3", LuaImGuiFunc::SliderVec3)
		LUA_LIB_ENTRY("SliderVec4", LuaImGuiFunc::SliderVec4)
		LUA_LIB_ENTRY("SliderRGB", LuaImGuiFunc::SliderRGB)
		LUA_LIB_ENTRY("SliderRGBA", LuaImGuiFunc::SliderRGBA)
		LUA_LIB_ENTRY("Button", LuaImGuiFunc::Button)
		LUA_LIB_ENTRY("Checkbox", LuaImGuiFunc::Checkbox)
		LUA_LIB_ENTRY("RadioButton", LuaImGuiFunc::RadioButton)
	LUA_LIB_END(ImGui)

	void LuaBindImGuiLib(lua_State * L) {
		LUA_REGISTER_LIB(ImGui);
	}

}
