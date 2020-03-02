
#ifndef __LUA_BIND_H__
#define __LUA_BIND_H__

#include <string>
#include <cstring>

#include <vm/lua.hpp>
#include <vm/vmlogger.h>

#include "luatypes.h"
#include "bindmath.h"

	struct LuaFunction {
		const char * Name;
		int(*Func)(lua_State * L);
	};

	#define LUA_LIB_START(N) static LuaFunction Lua##N##LibEntry[] = {
	#define LUA_LIB_ENTRY(N, F) { N, F },
	#define LUA_LIB_END(N) };

	#define LUA_REGISTER_LIB(N)\
		lua_newtable(L);\
		for (size_t i = 0; i < std::size(Lua##N##LibEntry); i++) {\
			lua_pushstring(L, Lua##N##LibEntry[i].Name);\
			lua_pushcfunction(L,  Lua##N##LibEntry[i].Func);\
			lua_settable(L, -3);\
		}\
		lua_setglobal(L, #N);

	#define LUA_METATABLE_START(N) static LuaFunction Lua##N##MetatableFunctions[] = {
	#define LUA_METATABLE_FUNCTION(N, F) { N, F },
	#define LUA_METATBLE_END(N) };
	
	#define LUA_SET_METATABLE(N)\
		if (lua_getglobal(L, #N) != LUA_TTABLE) {\
			LUA_LOG("No library with name %s found", #N);\
		} else {\
			luaL_newmetatable(L, #N "Metatable");\
			for (size_t i = 0; i < std::size(Lua##N##MetatableFunctions); i++) {\
				lua_pushstring(L, Lua##N##MetatableFunctions[i].Name);\
				lua_pushcfunction(L, Lua##N##MetatableFunctions[i].Func);\
				lua_settable(L, -3);\
			}\
			lua_setmetatable(L, -2);\
			lua_pop(L, 1);\
		}
	
	
	// ****************************************************************************************************** //
	
	#define LUA_CHECK_NUM_PARAMS(N) if (lua_gettop(L) != N) { LUA_DEBUG_LOG("Expected %d number of params but got %d", N, lua_gettop(L));\
									return 0; }

	#define LUA_CHECK_RANGE_PARAMS(MIN, MAX) if (lua_gettop(L) < MIN || lua_gettop(L) > MAX) {\
										LUA_DEBUG_LOG("Expected %d-%d number of params but got %d", MIN, MAX, lua_gettop(L));\
									return 0; }
	
	#define LUA_FLOAT_PARAM(P, V)	if (lua_type(L, P) != LUA_TNUMBER) { LUA_DEBUG_LOG("Expected float as parameter at index %d", P); return 0; }\
									float V = lua_tonumber(L, P)

	#define LUA_INT_PARAM(P, V)		if (lua_type(L, P) != LUA_TNUMBER) { LUA_DEBUG_LOG("Expected integer as parameter at index %d", P); return 0; }\
									int V = lua_tointeger(L, P)

	#define LUA_BOOL_PARAM(P, V)	if (lua_type(L, P) != LUA_TBOOLEAN) { LUA_DEBUG_LOG("Expected boolean as parameter at index %d", P); return 0; }\
									bool V = lua_toboolean(L, P)

	#define LUA_STRING_PARAM(P, V)	if (lua_type(L, P) != LUA_TSTRING) { LUA_DEBUG_LOG("Expected string as parameter at index %d", P); return 0; }\
									const char * V = lua_tostring(L, P)

	#define LUA_VEC2_PARAM(P, V)	if (lua_type(L, P) != LUA_TUSERDATA) { LUA_DEBUG_LOG("Expected Vec2 as parameter at index %d", P); return 0; }\
									lua_pushstring(L, "type");\
									lua_gettable(L, P);\
									if (lua_tointeger(L, -1) != TypeID::value<Vec2>()) {\
										LUA_DEBUG_LOG("Expected Vec2 as parameter at index %d", P); return 0;\
									}\
									lua_pop(L, 1);\
									Vec2 * V = (Vec2 *)lua_touserdata(L, P);

	#define LUA_VEC3_PARAM(P, V)	if (lua_type(L, P) != LUA_TUSERDATA) { LUA_DEBUG_LOG("Expected Vec3 as parameter at index %d", P); return 0; }\
									lua_pushstring(L, "type");\
									lua_gettable(L, P);\
									if (lua_tointeger(L, -1) != TypeID::value<Vec3>()) {\
										LUA_DEBUG_LOG("Expected Vec3 as parameter at index %d", P); return 0;\
									}\
									lua_pop(L, 1);\
									Vec3 * V = (Vec3 *)lua_touserdata(L, P);

	#define LUA_VEC4_PARAM(P, V)	if (lua_type(L, P) != LUA_TUSERDATA) { LUA_DEBUG_LOG("Expected Vec4 as parameter at index %d", P); return 0; }\
									lua_pushstring(L, "type");\
									lua_gettable(L, P);\
									if (lua_tointeger(L, -1) != TypeID::value<Vec4>()) {\
										LUA_DEBUG_LOG("Expected Vec4 as parameter at index %d", P); return 0;\
									}\
									lua_pop(L, 1);\
									Vec4 * V = (Vec4 *)lua_touserdata(L, P);

	#define LUA_CHECK_TABLE_PARAM(P) if (lua_type(L, P) != LUA_TTABLE) { LUA_DEBUG_LOG("Expected table as parameter at index %d", P); return 0; }\

	// ****************************************************************************************************** //

	#define LUA_PUSH_VEC2(X, Y) CreateVec2(L, X, Y)

	// ****************************************************************************************************** //

	#define LUA_SET_HANDLED_OBJECT_HANDLE(O) lua_pushstring(L, "handle"); lua_pushlightuserdata(L, O.Get()); lua_settable(L, -3); O->AddRef()
	#define LUA_SET_OBJECT_HANDLE(O) lua_pushstring(L, "handle"); lua_pushlightuserdata(L, O); lua_settable(L, -3)
	#define LUA_SET_CUSTOM_OBJECT_TYPE(T) lua_pushstring(L, "type"); lua_pushinteger(L, app::TypeID::value<T>()); lua_settable(L, -3)

	#define LUA_GENERATE_HANDLED_OBJECT_DESTRUCTOR(O)\
		LUA_METATABLE_FUNCTION("__gc", [](lua_State * L) -> int {\
			LUA_CHECK_NUM_PARAMS(1);\
			lua_pushstring(L, "handle");\
			lua_gettable(L, 1);\
			if (lua_type(L, -1) != LUA_TLIGHTUSERDATA) { LUA_LOG(#O " handle not set properly"); return 0; }\
			utils::StrongHandle<O> vb = (O *)lua_touserdata(L, -1);\
			vb->ReleaseRef();\
		})

	#define LUA_GENERATE_CUSTOM_OBJECT_DESTRUCTOR(O)\
		LUA_METATABLE_FUNCTION("__gc", [](lua_State * L) -> int {\
			LUA_CHECK_NUM_PARAMS(1);\
			lua_pushstring(L, "handle");\
			lua_gettable(L, 1);\
			if (lua_type(L, -1) != LUA_TLIGHTUSERDATA) { LUA_LOG(#O " handle not set properly"); return 0; }\
			O * vb = (O *)lua_touserdata(L, -1);\
			delete  vb;\
		})

	#define LUA_HANDLED_OBJECT_PARAM(P, T, V)\
		LUA_CHECK_TABLE_PARAM(P);\
		lua_pushstring(L, "type");\
		lua_gettable(L, P);\
		if (lua_type(L, -1) != LUA_TNUMBER) { LUA_DEBUG_LOG("Invalid parameter type at index %d", P); }\
		if (lua_tointeger(L, -1) != TypeID::value<T>()) { LUA_DEBUG_LOG("Invalid object type at index %d", P); }\
		lua_pop(L, 1);\
		lua_pushstring(L, "handle");\
		lua_gettable(L, P);\
		if (lua_type(L, -1) != LUA_TLIGHTUSERDATA) { LUA_DEBUG_LOG(#T " handle not set properly for object at index %d", P); return 0; }\
		utils::StrongHandle<T> V = (T *)lua_touserdata(L, -1);

	#define LUA_CUSTOM_OBJECT_PARAM(P, T, V)\
			LUA_CHECK_TABLE_PARAM(P);\
			lua_pushstring(L, "type");\
			lua_gettable(L, P);\
			if (lua_type(L, -1) != LUA_TNUMBER) { LUA_DEBUG_LOG("Invalid parameter type at index %d", P); }\
			if (lua_tointeger(L, -1) != app::TypeID::value<T>()) { LUA_DEBUG_LOG("Invalid object type at index %d", P); }\
			lua_pop(L, 1);\
			lua_pushstring(L, "handle");\
			lua_gettable(L, P);\
			if (lua_type(L, -1) != LUA_TLIGHTUSERDATA) { LUA_DEBUG_LOG(#T " handle not set properly for object at index %d", P); return 0; }\
			T * V = (T *)lua_touserdata(L, -1);

#endif //__LUA_BIND_H__
