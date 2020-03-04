#include "bindmath.h"
#include "luabind.h"
#include "luatypes.h"

#include <string>
#include <math/math.h>

namespace app {

	//---------------------------------------- VEC2 ----------------------------------------

	bool CreateVec2(lua_State * L, float x, float y) {
		Vec2 * v = (Vec2 *)lua_newuserdata(L, sizeof(Vec2));
		v->x = x;
		v->y = y;

		luaL_getmetatable(L, "LuaVec2Metatable");
		if (lua_type(L, -1) != LUA_TTABLE) { LUA_LOG("LuaVec2Metatable not found"); return false; }

		lua_setmetatable(L, -2);

		return true;
	}

	struct LuaVec2Func {
		
		static int New(lua_State * L) {
			LUA_CHECK_RANGE_PARAMS(0, 2);

			float x = 0.0f, y = 0.0f;

			if (lua_gettop(L) == 0) {
				x = 0.0f;
				y = 0.0f;
			} else if (lua_gettop(L) == 1) {
				x = lua_tonumber(L, 1);
				y = x;
			} else {
				x = lua_tonumber(L, 1);
				y = lua_tonumber(L, 2);
			}

			if (!CreateVec2(L, x, y)) return 0;
			else return 1;
		}

		static int __add(lua_State * L) {
			Vec2 * v1 = (Vec2 *)lua_touserdata(L, 1);
			Vec2 * v2 = (Vec2 *)lua_touserdata(L, 2);

			CreateVec2(L, v1->x + v2->x, v1->y + v2->y);

			return 1;
		}

		static int __sub(lua_State * L) {
			Vec2 * v1 = (Vec2 *)lua_touserdata(L, 1);
			Vec2 * v2 = (Vec2 *)lua_touserdata(L, 2);

			CreateVec2(L, v1->x - v2->x, v1->y - v2->y);

			return 1;
		}

		static int __mul(lua_State * L) {
			Vec2 * v1 = (Vec2 *)lua_touserdata(L, 1);
			Vec2 * v2 = (Vec2 *)lua_touserdata(L, 2);

			CreateVec2(L, v1->x * v2->x, v1->y * v2->y);

			return 1;
		}

		static int __div(lua_State * L) {
			Vec2 * v1 = (Vec2 *)lua_touserdata(L, 1);
			Vec2 * v2 = (Vec2 *)lua_touserdata(L, 2);

			CreateVec2(L, v1->x / v2->x, v1->y / v2->y);

			return 1;
		}

		static int __index(lua_State * L) {
			Vec2 * v = (Vec2 *)lua_touserdata(L, 1);
			LUA_STRING_PARAM(2, c);
			if (strcmp(c, "type") == 0) { lua_pushnumber(L, (lua_Number)TypeID::value<Vec2>()); return 1; }

			if (strlen(c) != 1) { LUA_LOG("Vec2 invalid index : %s", c); return 0; }

			switch (*c) {
			case 'x':
				lua_pushnumber(L, v->x); break;
			case 'y':
				lua_pushnumber(L, v->y); break;
			default:
			{
				LUA_LOG("Vec2 invalid index : %s", c);
				return 0;
				break;
			}
			}

			return 1;
		}

		static int __newindex(lua_State * L) {
			Vec2 * v = (Vec2 *)lua_touserdata(L, 1);
			LUA_STRING_PARAM(2, c);
			LUA_FLOAT_PARAM(3, val);
			if (strlen(c) != 1) { LUA_LOG("Vec2 invalid index : %s", c); return 0; }

			switch (*c) {
			case 'x':
				v->x = val; break;
			case 'y':
				v->y = val; break;
			default:
			{
				LUA_LOG("Vec2 invalid index : %s", c);
				break;
			}
			}

			return 0;
		}

		static int __tostring(lua_State * L) {
			Vec2 * v = (Vec2 *)lua_touserdata(L, 1);
			std::string s = "Vec2 : x = " + std::to_string(v->x) + ", y = " + std::to_string(v->y);
			lua_pushstring(L, s.c_str());
			return 1;
		}
	};

	LUA_LIB_START(Vec2)
		LUA_LIB_ENTRY("New", LuaVec2Func::New)
	LUA_LIB_END(Vec2)

	LUA_METATABLE_START(Vec2)
		LUA_METATABLE_FUNCTION("__add", LuaVec2Func::__add)
		LUA_METATABLE_FUNCTION("__sub", LuaVec2Func::__sub)
		LUA_METATABLE_FUNCTION("__mul", LuaVec2Func::__mul)
		LUA_METATABLE_FUNCTION("__div", LuaVec2Func::__div)
		LUA_METATABLE_FUNCTION("__index", LuaVec2Func::__index)
		LUA_METATABLE_FUNCTION("__newindex", LuaVec2Func::__newindex)
		LUA_METATABLE_FUNCTION("__tostring", LuaVec2Func::__tostring)
	LUA_METATBLE_END(Vec2)

	//---------------------------------------- VEC3 ----------------------------------------

	bool CreateVec3(lua_State * L, float x, float y, float z) {
		Vec3 * v = (Vec3 *)lua_newuserdata(L, sizeof(Vec3));
		v->x = x;
		v->y = y;
		v->z = z;

		luaL_getmetatable(L, "LuaVec3Metatable");
		if (lua_type(L, -1) != LUA_TTABLE) { LUA_LOG("LuaVec3Metatable not found"); return false; }

		lua_setmetatable(L, -2);

		return true;
	}

	struct LuaVec3Func {
		
		static int New(lua_State * L) {
			LUA_CHECK_RANGE_PARAMS(0, 3);

			float x = 0.0f, y = 0.0f, z = 0.0f;

			if (lua_gettop(L) == 0) {
				x = 0.0f;
				y = 0.0f;
			} else if (lua_gettop(L) == 1) {
				x = lua_tonumber(L, 1);
				y = x;
				z = y;
			} else if (lua_gettop(L) == 2) {
				x = lua_tonumber(L, 1);
				y = lua_tonumber(L, 2);
			} else {
				x = lua_tonumber(L, 1);
				y = lua_tonumber(L, 2);
				z = lua_tonumber(L, 3);
			}

			if (!CreateVec3(L, x, y, z)) return 0;
			else return 1;
		}

		static int __add(lua_State * L) {
			Vec3 * v1 = (Vec3 *)lua_touserdata(L, 1);
			Vec3 * v2 = (Vec3 *)lua_touserdata(L, 2);

			CreateVec3(L, v1->x + v2->x, v1->y + v2->y, v1->z + v2->z);

			return 1;
		}

		static int __sub(lua_State * L) {
			Vec3 * v1 = (Vec3 *)lua_touserdata(L, 1);
			Vec3 * v2 = (Vec3 *)lua_touserdata(L, 2);

			CreateVec3(L, v1->x - v2->x, v1->y - v2->y, v1->z - v2->z);

			return 1;
		}

		static int __mul(lua_State * L) {
			Vec3 * v1 = (Vec3 *)lua_touserdata(L, 1);
			Vec3 * v2 = (Vec3 *)lua_touserdata(L, 2);

			CreateVec3(L, v1->x * v2->x, v1->y * v2->y, v1->z * v2->z);

			return 1;
		}

		static int __div(lua_State * L) {
			Vec3 * v1 = (Vec3 *)lua_touserdata(L, 1);
			Vec3 * v2 = (Vec3 *)lua_touserdata(L, 2);

			CreateVec3(L, v1->x / v2->x, v1->y / v2->y, v1->z / v2->z);

			return 1;
		}

		static int __index(lua_State * L) {
			Vec3 * v = (Vec3 *)lua_touserdata(L, 1);
			LUA_STRING_PARAM(2, c);
			if (strcmp(c, "type") == 0) { lua_pushnumber(L, (lua_Number)TypeID::value<Vec3>()); return 1; }

			if (strlen(c) != 1) { LUA_LOG("Vec3 invalid index : %s", c); return 0; }

			switch (*c) {
			case 'x':
				lua_pushnumber(L, v->x); break;
			case 'y':
				lua_pushnumber(L, v->y); break;
			case 'z':
				lua_pushnumber(L, v->z); break;
			default:
			{
				LUA_LOG("Vec3 invalid index : %s", c);
				return 0;
				break;
			}
			}

			return 1;
		}

		static int __newindex(lua_State * L) {
			Vec3 * v = (Vec3 *)lua_touserdata(L, 1);
			LUA_STRING_PARAM(2, c);
			LUA_FLOAT_PARAM(3, val);
			if (strlen(c) != 1) { LUA_LOG("Vec3 invalid index : %s", c); return 0; }

			switch (*c) {
			case 'x':
				v->x = val; break;
			case 'y':
				v->y = val; break;
			case 'z':
				v->z = val; break;
			default:
			{
				LUA_LOG("Vec3 invalid index : %s", c);
				break;
			}
			}

			return 0;
		}

		static int __tostring(lua_State * L) {
			Vec3 * v = (Vec3 *)lua_touserdata(L, 1);
			std::string s = "Vec3 : x = " + std::to_string(v->x) + ", y = " + std::to_string(v->y) + ", z = " + std::to_string(v->z);
			lua_pushstring(L, s.c_str());
			return 1;
		}
	};

	LUA_LIB_START(Vec3)
		LUA_LIB_ENTRY("New", LuaVec3Func::New)
	LUA_LIB_END(Vec3)

	LUA_METATABLE_START(Vec3)
		LUA_METATABLE_FUNCTION("__add", LuaVec3Func::__add)
		LUA_METATABLE_FUNCTION("__sub", LuaVec3Func::__sub)
		LUA_METATABLE_FUNCTION("__mul", LuaVec3Func::__mul)
		LUA_METATABLE_FUNCTION("__div", LuaVec3Func::__div)
		LUA_METATABLE_FUNCTION("__index", LuaVec3Func::__index)
		LUA_METATABLE_FUNCTION("__newindex", LuaVec3Func::__newindex)
		LUA_METATABLE_FUNCTION("__tostring", LuaVec3Func::__tostring)
	LUA_METATBLE_END(Vec3)

	//---------------------------------------- VEC4 ----------------------------------------

	bool CreateVec4(lua_State * L, float x, float y, float z, float w) {
		Vec4 * v = (Vec4 *)lua_newuserdata(L, sizeof(Vec4));
		v->x = x;
		v->y = y;
		v->z = z;
		v->w = w;

		luaL_getmetatable(L, "LuaVec4Metatable");
		if (lua_type(L, -1) != LUA_TTABLE) { LUA_LOG("LuaVec4Metatable not found"); return false; }

		lua_setmetatable(L, -2);

		return true;
	}

	struct LuaVec4Func {
		
		static int New(lua_State * L) {
			LUA_CHECK_RANGE_PARAMS(0, 4);

			float x = 0.0f, y = 0.0f, z = 0.0f, w = 0.0f;

			if (lua_gettop(L) == 0) {
				x = 0.0f;
				y = 0.0f;
			} else if (lua_gettop(L) == 1) {
				x = lua_tonumber(L, 1);
				y = x;
				z = y;
				w = z;
			} else if (lua_gettop(L) == 2) {
				x = lua_tonumber(L, 1);
				y = lua_tonumber(L, 2);
			} else if (lua_gettop(L) == 3){
				x = lua_tonumber(L, 1);
				y = lua_tonumber(L, 2);
				z = lua_tonumber(L, 3);
			} else {
				x = lua_tonumber(L, 1);
				y = lua_tonumber(L, 2);
				z = lua_tonumber(L, 3);
				w = lua_tonumber(L, 4);
			}

			if (!CreateVec4(L, x, y, z, w)) return 0;
			else return 1;
		}

		static int __add(lua_State * L) {
			Vec4 * v1 = (Vec4 *)lua_touserdata(L, 1);
			Vec4 * v2 = (Vec4 *)lua_touserdata(L, 2);

			CreateVec4(L, v1->x + v2->x, v1->y + v2->y, v1->z + v2->z, v1->w + v2->w);

			return 1;
		}

		static int __sub(lua_State * L) {
			Vec4 * v1 = (Vec4 *)lua_touserdata(L, 1);
			Vec4 * v2 = (Vec4 *)lua_touserdata(L, 2);

			CreateVec4(L, v1->x - v2->x, v1->y - v2->y, v1->z - v2->z, v1->w - v2->w);

			return 1;
		}

		static int __mul(lua_State * L) {
			Vec4 * v1 = (Vec4 *)lua_touserdata(L, 1);
			Vec4 * v2 = (Vec4 *)lua_touserdata(L, 2);

			CreateVec4(L, v1->x * v2->x, v1->y * v2->y, v1->z * v2->z, v1->w * v2->w);

			return 1;
		}

		static int __div(lua_State * L) {
			Vec4 * v1 = (Vec4 *)lua_touserdata(L, 1);
			Vec4 * v2 = (Vec4 *)lua_touserdata(L, 2);

			CreateVec4(L, v1->x / v2->x, v1->y / v2->y, v1->z / v2->z, v1->w / v2->w);

			return 1;
		}

		static int __index(lua_State * L) {
			Vec4 * v = (Vec4 *)lua_touserdata(L, 1);
			LUA_STRING_PARAM(2, c);
			if (strcmp(c, "type") == 0) { lua_pushnumber(L, (lua_Number)TypeID::value<Vec4>()); return 1; }

			if (strlen(c) != 1) { LUA_LOG("Vec4 invalid index : %s", c); return 0; }

			switch (*c) {
			case 'x':
				lua_pushnumber(L, v->x); break;
			case 'y':
				lua_pushnumber(L, v->y); break;
			case 'z':
				lua_pushnumber(L, v->z); break;
			case 'w':
				lua_pushnumber(L, v->w); break;
			default:
			{
				LUA_LOG("Vec4 invalid index : %s", c);
				return 0;
				break;
			}
			}

			return 1;
		}

		static int __newindex(lua_State * L) {
			Vec4 * v = (Vec4 *)lua_touserdata(L, 1);
			LUA_STRING_PARAM(2, c);
			LUA_FLOAT_PARAM(3, val);
			if (strlen(c) != 1) { LUA_LOG("Vec4 invalid index : %s", c); return 0; }

			switch (*c) {
			case 'x':
				v->x = val; break;
			case 'y':
				v->y = val; break;
			case 'z':
				v->z = val; break;
			case 'w':
				v->w = val; break;
			default:
			{
				LUA_LOG("Vec4 invalid index : %s", c);
				break;
			}
			}

			return 0;
		}

		static int __tostring(lua_State * L) {
			Vec4 * v = (Vec4 *)lua_touserdata(L, 1);
			std::string s = "Vec4 : x = " + std::to_string(v->x) + ", y = " + 
				std::to_string(v->y) + ", z = " + std::to_string(v->z) + ", w = " + std::to_string(v->w);
			lua_pushstring(L, s.c_str());
			return 1;
		}
	};

	LUA_LIB_START(Vec4)
		LUA_LIB_ENTRY("New", LuaVec4Func::New)
	LUA_LIB_END(Vec4)

	LUA_METATABLE_START(Vec4)
		LUA_METATABLE_FUNCTION("__add", LuaVec4Func::__add)
		LUA_METATABLE_FUNCTION("__sub", LuaVec4Func::__sub)
		LUA_METATABLE_FUNCTION("__mul", LuaVec4Func::__mul)
		LUA_METATABLE_FUNCTION("__div", LuaVec4Func::__div)
		LUA_METATABLE_FUNCTION("__index", LuaVec4Func::__index)
		LUA_METATABLE_FUNCTION("__newindex", LuaVec4Func::__newindex)
		LUA_METATABLE_FUNCTION("__tostring", LuaVec4Func::__tostring)
	LUA_METATBLE_END(Vec4)

	void LuaBindMath(lua_State * L) {

		// VEC2

		luaL_newmetatable(L, "LuaVec2Metatable");
		for (size_t i = 0; i < std::size(LuaVec2MetatableFunctions); i++) {
			lua_pushstring(L, LuaVec2MetatableFunctions[i].Name);
			lua_pushcfunction(L, LuaVec2MetatableFunctions[i].Func);
			lua_settable(L, -3);
		}
		lua_pop(L, 1);

		// VEC3

		luaL_newmetatable(L, "LuaVec3Metatable");
		for (size_t i = 0; i < std::size(LuaVec3MetatableFunctions); i++) {
			lua_pushstring(L, LuaVec3MetatableFunctions[i].Name);
			lua_pushcfunction(L, LuaVec3MetatableFunctions[i].Func);
			lua_settable(L, -3);
		}
		lua_pop(L, 1);

		// VEC4

		luaL_newmetatable(L, "LuaVec4Metatable");
		for (size_t i = 0; i < std::size(LuaVec4MetatableFunctions); i++) {
			lua_pushstring(L, LuaVec4MetatableFunctions[i].Name);
			lua_pushcfunction(L, LuaVec4MetatableFunctions[i].Func);
			lua_settable(L, -3);
		}
		lua_pop(L, 1);

		LUA_REGISTER_LIB(Vec2);
		LUA_REGISTER_LIB(Vec3);
		LUA_REGISTER_LIB(Vec4);
	}
}
