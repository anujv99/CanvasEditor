
#ifndef __BIND_MATH_H__
#define __BIND_MATH_H__

struct lua_State;

namespace app {
	bool CreateVec2(lua_State * L, float x, float y);
	bool CreateVec3(lua_State * L, float x, float y, float z);
	bool CreateVec4(lua_State * L, float x, float y, float z, float w);
	void LuaBindMath(lua_State * L);

}

#endif //__BIND_MATH_H__
