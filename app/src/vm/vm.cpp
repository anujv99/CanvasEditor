#include "vm.h"

#include "lua.hpp"

#include "utils/fileutils.h"
#include "vmlogger.h"

#include "luabind/bindimguilib.h"
#include "luabind/bindrendererlib.h"
#include "luabind/bindmath.h"
#include "luabind/bindgraphicslib.h"
#include "luabind/bindcorelib.h"
#include "luabind/bindsystemlib.h"

namespace app { namespace vm {

	VM::VM() : L(nullptr), m_UpdateFunction(-1), m_RenderFunction(-1), m_RegistryTableIndex(-1), m_GuiFunction(-1) {
		L = luaL_newstate();
		luaL_openlibs(L);

		LuaBindImGuiLib(L);
		LuaBindMath(L);
		LuaBindRendererLib(L);
		LuaBindGraphicsLib(L);
		LuaBindCoreLib(L);
		LuaBindSystemLib(L);
	}

	VM::~VM() {
		lua_close(L);
	}

	void VM::Initialize(const char * mainFile) {

		if (!utils::FileUtils::FileExists(mainFile)) {
			LOG_ERROR("Failed to find script file : %s", mainFile);
		}

		if (luaL_dofile(L, mainFile) != LUA_OK) {
			LOG_ERROR("LUA ERROR : %s", lua_tostring(L, -1));
			return;
		}

		lua_getglobal(L, "g_main_thread");
		ASSERTM(lua_type(L, -1) == LUA_TTABLE, "Invalid script file");

		lua_pushstring(L, "update");
		lua_gettable(L, -2);
		ASSERTM(lua_type(L, -1) == LUA_TFUNCTION, "main_thread table should contain update function");
		m_UpdateFunction = luaL_ref(L, LUA_REGISTRYINDEX);

		lua_pushstring(L, "render");
		lua_gettable(L, -2);
		ASSERTM(lua_type(L, -1) == LUA_TFUNCTION, "main_thread table should contain render function");
		m_RenderFunction = luaL_ref(L, LUA_REGISTRYINDEX);

		lua_pushstring(L, "gui");
		lua_gettable(L, -2);
		ASSERTM(lua_type(L, -1) == LUA_TFUNCTION, "main_thread table should contain gui function");
		m_GuiFunction = luaL_ref(L, LUA_REGISTRYINDEX);

		lua_pop(L, 1);

	}

	void VM::Update(float dt) {
		lua_rawgeti(L, LUA_REGISTRYINDEX, m_UpdateFunction);
		lua_getglobal(L, "g_main_thread");
		lua_pushnumber(L, dt);
		int err = lua_pcall(L, 2, 0, 0);
		if (err != LUA_OK) {
			LOG_ERROR("LUA ERROR : %s", lua_tostring(L, -1));
		}
	}

	void VM::Render() {
		lua_rawgeti(L, LUA_REGISTRYINDEX, m_RenderFunction);
		lua_getglobal(L, "g_main_thread");
		int err = lua_pcall(L, 1, 0, 0);
		if (err != LUA_OK) {
			LOG_ERROR("LUA ERROR : %s", lua_tostring(L, -1));
		}
	}

	void VM::Gui() {
		lua_rawgeti(L, LUA_REGISTRYINDEX, m_GuiFunction);
		lua_getglobal(L, "g_main_thread");
		int err = lua_pcall(L, 1, 0, 0);
		if (err != LUA_OK) {
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
