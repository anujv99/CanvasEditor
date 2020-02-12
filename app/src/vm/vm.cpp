#include "vm.h"

#include "lua.hpp"

#include "utils/fileutils.h"
#include "vmlogger.h"

#include "luabind/bindimguilib.h"
#include "luabind/bindrendererlib.h"
#include "luabind/bindmath.h"

namespace app { namespace vm {

	VM::VM() : L(nullptr), S(nullptr) {
		L = luaL_newstate();
		luaL_openlibs(L);

		LuaBindImGuiLib(L);
		LuaBindMath(L);
		LuaBindRendererLib(L);
	}

	VM::~VM() {
		lua_close(L);
	}

	void VM::Run(const char * mainFile) {

		if (!utils::FileUtils::FileExists(mainFile)) {
			LOG_ERROR("Failed to find script file : %s", mainFile);
		}

		if (luaL_dofile(L, mainFile) != LUA_OK) {
			LOG_ERROR("LUA ERROR : %s", lua_tostring(L, -1));
			return;
		}

		lua_getglobal(L, "main_thread");
		S = lua_tothread(L, -1);
		lua_pop(L, 1);
	}

	void VM::Update() {
		static int res = 0;
		int r = lua_resume(S, L, 0, &res);
		if (r != LUA_OK && r != LUA_YIELD) {
			LOG_ERROR("LUA ERROR : %s", lua_tostring(L, -1));
		}
	}

	void VM::DoString(const char * str) {
		if (luaL_dostring(L, str) != LUA_OK) {
			LOG_ERROR("LUA ERROR : %s", lua_tostring(L, -1));
			lua_pop(L, 1);
		}
	}

	WindowConfig VM::ReadConfigFile(const char * path) {

		WindowConfig c;

		if (!utils::FileUtils::FileExists(path)) {
			LOG_ERROR("Failed to find config file : %s", path);
			return c;
		}

		utils::StrongHandle<utils::DataFile> file = utils::FileUtils::OpenFile(path);

		DoString(file->GetContent().c_str());

		lua_getglobal(L, "Config");
		if (lua_type(L, -1) != LUA_TTABLE) { LOG_ERROR("Invalid config file : %s", path); return c; }

		lua_pushstring(L, "Width");
		lua_gettable(L, -2);
		if (lua_type(L, -1) == LUA_TNUMBER) { c.Width = lua_tointeger(L, -1); lua_pop(L, 1); }

		lua_pushstring(L, "Height");
		lua_gettable(L, -2);
		if (lua_type(L, -1) == LUA_TNUMBER) { c.Height = lua_tointeger(L, -1); lua_pop(L, 1); }

		lua_pushstring(L, "Title");
		lua_gettable(L, -2);
		if (lua_type(L, -1) == LUA_TSTRING) { c.Title = lua_tostring(L, -1); lua_pop(L, 1); }

		lua_pop(L, 1);

		return c;
	}

} }
