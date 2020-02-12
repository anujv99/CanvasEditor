
#ifndef __LUA_BIND_H__
#define __LUA_BIND_H__

#include <cstring>

#include <vm/lua.hpp>
#include <vm/vmlogger.h>

#include "luatypes.h"
#include "bindmath.h"

namespace app {

	struct LuaFunction {
		const char * Name;
		int(*Func)(lua_State * L);
	};

	#define LUA_LIB_START(N) static app::LuaFunction Lua##N##LibEntry[] = {
	#define LUA_LIB_ENTRY(N, F) { N, F },
	#define LUA_LIB_END(N) };

	#define LUA_REGISTER_LIB(N)\
		lua_newtable(L);\
		for (size_t i = 0; i < std::size(app::Lua##N##LibEntry); i++) {\
			lua_pushstring(L, app::Lua##N##LibEntry[i].Name);\
			lua_pushcfunction(L,  app::Lua##N##LibEntry[i].Func);\
			lua_settable(L, -3);\
		}\
		lua_setglobal(L, #N);

	#define LUA_METATABLE_START(N) static app::LuaFunction Lua##N##MetatableFunctions[] = {
	#define LUA_METATABLE_FUNCTION(N, F) { N, F },
	#define LUA_METATBLE_END(N) };
	
	#define LUA_SET_METATABLE(N)\
		lua_getglobal(L, #N);\
		if (lua_type(L, -1) != LUA_TTABLE) {\
			LOG_ERROR("No library with name %s found", #N);\
		} else {\
			luaL_newmetatable(L, #N "Metatable");\
			for (size_t i = 0; i < std::size(app::Lua##N##MetatableFunctions); i++) {\
				lua_pushstring(L, app::Lua##N##MetatableFunctions[i].Name);\
				lua_pushcfunction(L, Lua##N##MetatableFunctions[i].Func);\
				lua_settable(L, -3);\
			}\
			lua_setmetatable(L, -2);\
		}
	
	
	// ****************************************************************************************************** //
	
	#define LUA_CHECK_NUM_PARAMS(N) if (lua_gettop(L) != N) { LUA_LOG("Expected %d number of params but got %d", N, lua_gettop(L));\
									return 0; }

	#define LUA_CHECK_RANGE_PARAMS(MIN, MAX) if (lua_gettop(L) < MIN || lua_gettop(L) > MAX) {\
										LUA_LOG("Expected %d-%d number of params but got %d", MIN, MAX, lua_gettop(L));\
									return 0; }
	
	#define LUA_FLOAT_PARAM(P, V)	if (lua_type(L, P) != LUA_TNUMBER) { LUA_LOG("Expected integer as parameter at index %d", P); return 0; }\
									float V = lua_tonumber(L, P)

	#define LUA_INT_PARAM(P, V)		if (lua_type(L, P) != LUA_TNUMBER) { LUA_LOG("Expected integer as parameter at index %d", P); return 0; }\
									int V = lua_tointeger(L, P)

	#define LUA_STRING_PARAM(P, V)	if (lua_type(L, P) != LUA_TSTRING) { LUA_LOG("Expected string as parameter at index %d", P); return 0; }\
									const char * V = lua_tostring(L, P)

	#define LUA_VEC2_PARAM(P, V)	if (lua_type(L, P) != LUA_TUSERDATA) { LUA_LOG("Expected Vec2 as parameter at index %d", P); return 0; }\
									lua_pushstring(L, "type");\
									lua_gettable(L, P);\
									if (lua_tointeger(L, -1) != LUA_VEC2_TYPE) { LUA_LOG("Expected Vec2 as parameter at index %d", P); return 0; }\
									lua_pop(L, 1);\
									Vec2 * V = (Vec2 *)lua_touserdata(L, P);

	#define LUA_VEC3_PARAM(P, V)	if (lua_type(L, P) != LUA_TUSERDATA) { LUA_LOG("Expected Vec3 as parameter at index %d", P); return 0; }\
									lua_pushstring(L, "type");\
									lua_gettable(L, P);\
									if (lua_tointeger(L, -1) != LUA_VEC3_TYPE) { LUA_LOG("Expected Vec3 as parameter at index %d", P); return 0; }\
									lua_pop(L, 1);\
									Vec3 * V = (Vec3 *)lua_touserdata(L, P);

	#define LUA_VEC4_PARAM(P, V)	if (lua_type(L, P) != LUA_TUSERDATA) { LUA_LOG("Expected Vec4 as parameter at index %d", P); return 0; }\
									lua_pushstring(L, "type");\
									lua_gettable(L, P);\
									if (lua_tointeger(L, -1) != LUA_VEC4_TYPE) { LUA_LOG("Expected Vec4 as parameter at index %d", P); return 0; }\
									lua_pop(L, 1);\
									Vec4 * V = (Vec4 *)lua_touserdata(L, P);

	// ****************************************************************************************************** //

	#define LUA_PUSH_VEC2(X, Y) CreateVec2(L, X, Y)


}

#endif //__LUA_BIND_H__
