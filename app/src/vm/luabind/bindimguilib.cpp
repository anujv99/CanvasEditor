#include "bindimguilib.h"

#include "luabind.h"
#include <imgui/imgui.h>

namespace app {

	struct LuaImGuiFunc {
		static int Begin(lua_State * L) {
			LUA_CHECK_NUM_PARAMS(1);
			LUA_STRING_PARAM(1, name);
			ImGui::Begin(name);
			return 0;
		}

		static int TextInput(lua_State * L) {
			LUA_CHECK_NUM_PARAMS(3);
			LUA_STRING_PARAM(1, name);
			LUA_STRING_PARAM(2, text);
			LUA_INT_PARAM(3, width);

			std::string str(text);

			ImGui::TextInput(name, str, width);

			lua_pushstring(L, str.c_str());
			return 1;
		}

		static int End(lua_State * L) {
			LUA_CHECK_NUM_PARAMS(0);
			ImGui::End();
			return 0;
		}

		static int SliderFloat(lua_State * L) {
			LUA_CHECK_NUM_PARAMS(4);
			LUA_STRING_PARAM(1, name);
			LUA_FLOAT_PARAM(2, val);
			LUA_FLOAT_PARAM(3, min);
			LUA_FLOAT_PARAM(4, max);
			lua_pushnumber(L, ImGui::SliderFloat(name, val, min, max));
			return 1;
		}

		static int SliderInt(lua_State * L) {
			LUA_CHECK_NUM_PARAMS(4);
			LUA_STRING_PARAM(1, name);
			LUA_INT_PARAM(2, val);
			LUA_INT_PARAM(3, min);
			LUA_INT_PARAM(4, max);

			lua_pushnumber(L, (lua_Number)ImGui::SliderInt(name, val, min, max));
			return 1;
		}

		static int SliderVec2(lua_State * L) {
			LUA_CHECK_NUM_PARAMS(4);
			LUA_STRING_PARAM(1, name);
			LUA_VEC2_PARAM(2, val);
			LUA_VEC2_PARAM(3, min);
			LUA_VEC2_PARAM(4, max);

			ImGui::SliderVec2(name, *val, *min, *max);

			return 0;
		}

		static int SliderVec3(lua_State * L) {
			LUA_CHECK_NUM_PARAMS(4);
			LUA_STRING_PARAM(1, name);
			LUA_VEC3_PARAM(2, val);
			LUA_VEC3_PARAM(3, min);
			LUA_VEC3_PARAM(4, max);

			ImGui::SliderVec3(name, *val, *min, *max);

			return 0;
		}

		static int SliderVec4(lua_State * L) {
			LUA_CHECK_NUM_PARAMS(4);
			LUA_STRING_PARAM(1, name);
			LUA_VEC4_PARAM(2, val);
			LUA_VEC4_PARAM(3, min);
			LUA_VEC4_PARAM(4, max);

			ImGui::SliderVec4(name, *val, *min, *max);

			return 0;
		}

		static int SliderRGB(lua_State * L) {
			LUA_CHECK_NUM_PARAMS(2);
			LUA_STRING_PARAM(1, name);
			LUA_VEC3_PARAM(2, val);

			ImGui::SliderRGB(name, *val);

			return 0;
		}

		static int Button(lua_State * L) {
			LUA_CHECK_NUM_PARAMS(1);
			LUA_STRING_PARAM(1, name);
			lua_pushboolean(L, (int)ImGui::Button(name));
			return 1;
		}
	};

	LUA_LIB_START(ImGui)
		LUA_LIB_ENTRY("Begin", LuaImGuiFunc::Begin)
		LUA_LIB_ENTRY("End", LuaImGuiFunc::End)
		LUA_LIB_ENTRY("TextInput", LuaImGuiFunc::TextInput)
		LUA_LIB_ENTRY("SliderFloat", LuaImGuiFunc::SliderFloat)
		LUA_LIB_ENTRY("SliderInt", LuaImGuiFunc::SliderInt)
		LUA_LIB_ENTRY("SliderVec2", LuaImGuiFunc::SliderVec2)
		LUA_LIB_ENTRY("SliderVec3", LuaImGuiFunc::SliderVec3)
		LUA_LIB_ENTRY("SliderVec4", LuaImGuiFunc::SliderVec4)
		LUA_LIB_ENTRY("SliderRGB", LuaImGuiFunc::SliderRGB)
		LUA_LIB_ENTRY("Button", LuaImGuiFunc::Button)
	LUA_LIB_END(ImGui)

	void LuaBindImGuiLib(lua_State * L) {
		LUA_REGISTER_LIB(ImGui);
	}

}
