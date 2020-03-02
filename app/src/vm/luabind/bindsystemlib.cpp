#include "bindsystemlib.h"
#include "luabind.h"
#include "luatypes.h"

#include <utils/fileutils.h>

namespace app {

	struct LuaBindSystemFunc {
		static int DoFile(lua_State * L) {
			LUA_CHECK_NUM_PARAMS(1);
			LUA_STRING_PARAM(1, path);
			std::string newPath = utils::FileUtils::ConvertToRelativePath(path);
			if (!utils::FileUtils::FileExists(newPath.c_str())) { LUA_DEBUG_LOG("File does not exists : %s", path); }
			if (luaL_dofile(L, newPath.c_str()) != LUA_OK) {
				LUA_DEBUG_LOG("Script Error : %s\n Error : %s", path, lua_tostring(L, -1));
				lua_pop(L, 1);
			}
			return lua_gettop(L) - 1;
		}
	};

	LUA_LIB_START(System)
		LUA_LIB_ENTRY("DoFile", LuaBindSystemFunc::DoFile)
	LUA_LIB_END(System)

	void LuaBindSystemLib(lua_State * L) {
		LUA_REGISTER_LIB(System);
	}

}
