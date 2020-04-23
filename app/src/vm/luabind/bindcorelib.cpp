#include "bindcorelib.h"
#include "luabind.h"

#include <core/input.h>

namespace app {

	void SetKeyCode(lua_State * L, const char * str, int code) {
		lua_pushstring(L, str);
		lua_pushinteger(L, (int)code);
		lua_settable(L, -3);
	}

	#define SET_KEY_CODE(C) SetKeyCode(L, #C, C)

	using namespace core;

	struct LuaBindInputFunc {
		static int IsKeyDown(lua_State * L) {
			LUA_CHECK_NUM_PARAMS(1);
			LUA_INT_PARAM(1, keyCode);
			lua_pushboolean(L, Input::IsKeyDown((unsigned short)keyCode));
			return 1;
		}

		static int IsKeyPressed(lua_State * L) {
			LUA_CHECK_NUM_PARAMS(1);
			LUA_INT_PARAM(1, keyCode);
			lua_pushboolean(L, Input::IsKeyPressed((unsigned short)keyCode));
			return 1;
		}

		static int IsMouseKeyDown(lua_State * L) {
			LUA_CHECK_NUM_PARAMS(1);
			LUA_INT_PARAM(1, buttonCode);
			lua_pushboolean(L, Input::IsMouseButtonDown((unsigned short)buttonCode));
			return 1;
		}

		static int __index(lua_State * L) {
			LUA_CHECK_NUM_PARAMS(2);
			LUA_STRING_PARAM(2, s);
			if (strcmp(s, "MousePos") == 0) {
				auto pos = Input::GetMousePosition();
				LUA_PUSH_VEC2(pos.x, pos.y);
				return 1;
			} else if (strcmp(s, "MouseDeltaPos") == 0) {
				auto pos = Input::GetMouseDeltaPosition();
				LUA_PUSH_VEC2(pos.x, pos.y);
				return 1;
			} else if (strcmp(s, "RawMousePos") == 0) {
				auto pos = Input::GetRawMousePos();
				LUA_PUSH_VEC2(pos.x, pos.y);
				return 1;
			} else {
				LUA_DEBUG_LOG("Invalid index : %s", s);
			}

			return 0;
		}
	};

	LUA_LIB_START(Input)
		LUA_LIB_ENTRY("IsKeyDown", LuaBindInputFunc::IsKeyDown)
		LUA_LIB_ENTRY("IsKeyPressed", LuaBindInputFunc::IsKeyPressed)
		LUA_LIB_ENTRY("IsMouseKeyDown", LuaBindInputFunc::IsMouseKeyDown)
	LUA_LIB_END(Input)

	LUA_METATABLE_START(Input)
		LUA_METATABLE_FUNCTION("__index", LuaBindInputFunc::__index)
	LUA_METATBLE_END(Input)


	void LuaBindInputLib(lua_State * L) {
		LUA_REGISTER_LIB(Input);
		LUA_SET_METATABLE(Input);

		if (lua_getglobal(L, "Input") == LUA_TTABLE) {
			SET_KEY_CODE(KEY_SPACE);
			SET_KEY_CODE(KEY_APOSTROPHE);
			SET_KEY_CODE(KEY_COMMA);
			SET_KEY_CODE(KEY_MINUS);
			SET_KEY_CODE(KEY_PERIOD);
			SET_KEY_CODE(KEY_SLASH);
			SET_KEY_CODE(KEY_0);
			SET_KEY_CODE(KEY_1);
			SET_KEY_CODE(KEY_2);
			SET_KEY_CODE(KEY_3);
			SET_KEY_CODE(KEY_4);
			SET_KEY_CODE(KEY_5);
			SET_KEY_CODE(KEY_6);
			SET_KEY_CODE(KEY_7);
			SET_KEY_CODE(KEY_8);
			SET_KEY_CODE(KEY_9);
			SET_KEY_CODE(KEY_SEMICOLON);
			SET_KEY_CODE(KEY_EQUAL);
			SET_KEY_CODE(KEY_A);
			SET_KEY_CODE(KEY_B);
			SET_KEY_CODE(KEY_C);
			SET_KEY_CODE(KEY_D);
			SET_KEY_CODE(KEY_E);
			SET_KEY_CODE(KEY_F);
			SET_KEY_CODE(KEY_G);
			SET_KEY_CODE(KEY_H);
			SET_KEY_CODE(KEY_I);
			SET_KEY_CODE(KEY_J);
			SET_KEY_CODE(KEY_K);
			SET_KEY_CODE(KEY_L);
			SET_KEY_CODE(KEY_M);
			SET_KEY_CODE(KEY_N);
			SET_KEY_CODE(KEY_O);
			SET_KEY_CODE(KEY_P);
			SET_KEY_CODE(KEY_Q);
			SET_KEY_CODE(KEY_R);
			SET_KEY_CODE(KEY_S);
			SET_KEY_CODE(KEY_T);
			SET_KEY_CODE(KEY_U);
			SET_KEY_CODE(KEY_V);
			SET_KEY_CODE(KEY_W);
			SET_KEY_CODE(KEY_X);
			SET_KEY_CODE(KEY_Y);
			SET_KEY_CODE(KEY_Z);
			SET_KEY_CODE(KEY_LEFT_BRACKET);
			SET_KEY_CODE(KEY_BACKSLASH);
			SET_KEY_CODE(KEY_RIGHT_BRACKET);
			SET_KEY_CODE(KEY_GRAVE_ACCENT);

			SET_KEY_CODE(KEY_ESCAPE);
			SET_KEY_CODE(KEY_ENTER);
			SET_KEY_CODE(KEY_TAB);
			SET_KEY_CODE(KEY_BACKSPACE);
			SET_KEY_CODE(KEY_INSERT);
			SET_KEY_CODE(KEY_DELETE);
			SET_KEY_CODE(KEY_RIGHT);
			SET_KEY_CODE(KEY_LEFT);
			SET_KEY_CODE(KEY_DOWN);
			SET_KEY_CODE(KEY_UP);
			SET_KEY_CODE(KEY_PAGE_UP);
			SET_KEY_CODE(KEY_PAGE_DOWN);
			SET_KEY_CODE(KEY_HOME);
			SET_KEY_CODE(KEY_END);
			SET_KEY_CODE(KEY_CAPS_LOCK);
			SET_KEY_CODE(KEY_SCROLL_LOCK);
			SET_KEY_CODE(KEY_NUM_LOCK);
			SET_KEY_CODE(KEY_PRINT_SCREEN);
			SET_KEY_CODE(KEY_PAUSE);
			SET_KEY_CODE(KEY_F1);
			SET_KEY_CODE(KEY_F2);
			SET_KEY_CODE(KEY_F3);
			SET_KEY_CODE(KEY_F4);
			SET_KEY_CODE(KEY_F5);
			SET_KEY_CODE(KEY_F6);
			SET_KEY_CODE(KEY_F7);
			SET_KEY_CODE(KEY_F8);
			SET_KEY_CODE(KEY_F9);
			SET_KEY_CODE(KEY_F10);
			SET_KEY_CODE(KEY_F11);
			SET_KEY_CODE(KEY_F12);
			SET_KEY_CODE(KEY_F13);
			SET_KEY_CODE(KEY_F14);
			SET_KEY_CODE(KEY_F15);
			SET_KEY_CODE(KEY_F16);
			SET_KEY_CODE(KEY_F17);
			SET_KEY_CODE(KEY_F18);
			SET_KEY_CODE(KEY_F19);
			SET_KEY_CODE(KEY_F20);
			SET_KEY_CODE(KEY_F21);
			SET_KEY_CODE(KEY_F22);
			SET_KEY_CODE(KEY_F23);
			SET_KEY_CODE(KEY_F24);
			SET_KEY_CODE(KEY_F25);
			SET_KEY_CODE(KEY_KP_0);
			SET_KEY_CODE(KEY_KP_1);
			SET_KEY_CODE(KEY_KP_2);
			SET_KEY_CODE(KEY_KP_3);
			SET_KEY_CODE(KEY_KP_4);
			SET_KEY_CODE(KEY_KP_5);
			SET_KEY_CODE(KEY_KP_6);
			SET_KEY_CODE(KEY_KP_7);
			SET_KEY_CODE(KEY_KP_8);
			SET_KEY_CODE(KEY_KP_9);
			SET_KEY_CODE(KEY_KP_DECIMAL);
			SET_KEY_CODE(KEY_KP_DIVIDE);
			SET_KEY_CODE(KEY_KP_MULTIPLY);
			SET_KEY_CODE(KEY_KP_SUBTRACT);
			SET_KEY_CODE(KEY_KP_ADD);
			SET_KEY_CODE(KEY_KP_ENTER);
			SET_KEY_CODE(KEY_KP_EQUAL);
			SET_KEY_CODE(KEY_LEFT_SHIFT);
			SET_KEY_CODE(KEY_LEFT_CONTROL);
			SET_KEY_CODE(KEY_LEFT_ALT);
			SET_KEY_CODE(KEY_LEFT_SUPER);
			SET_KEY_CODE(KEY_RIGHT_SHIFT);
			SET_KEY_CODE(KEY_RIGHT_CONTROL);
			SET_KEY_CODE(KEY_RIGHT_ALT);
			SET_KEY_CODE(KEY_RIGHT_SUPER);
			SET_KEY_CODE(KEY_MENU);

			SET_KEY_CODE(MOUSE_BUTTON_1);
			SET_KEY_CODE(MOUSE_BUTTON_2);
			SET_KEY_CODE(MOUSE_BUTTON_3);
			SET_KEY_CODE(MOUSE_BUTTON_4);
			SET_KEY_CODE(MOUSE_BUTTON_5);
			SET_KEY_CODE(MOUSE_BUTTON_LEFT);
			SET_KEY_CODE(MOUSE_BUTTON_RIGHT);
			SET_KEY_CODE(MOUSE_BUTTON_MIDDLE);

		}
	}

	//----------------WINDOW----------------

	struct LuaBindWindowFunc {

	};

	void LuaBindCoreLib(lua_State * L) {
		LuaBindInputLib(L);
	}

}
