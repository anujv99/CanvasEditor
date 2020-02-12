#include "bindgraphicslib.h"
#include "luabind.h"

#include "graphics/renderstate.h"

namespace app {

	using namespace graphics;

	struct LuaBindRenderStateFunc {
		static int __index(lua_State * L) {
			return 0;
		}
		static int __newindex(lua_State * L) {
			return 0;
		}
	};

	LUA_METATABLE_START(RenderState)
		LUA_METATABLE_FUNCTION("__index", LuaBindRenderStateFunc::__index)
		LUA_METATABLE_FUNCTION("__newindex", LuaBindRenderStateFunc::__newindex)
	LUA_METATBLE_END(RenderState)

	void BindRenderStateLib(lua_State * L) {

	}

	void LuaBindGraphicsLib(lua_State * L) {
		
	}

}


