#include "bindrendererlib.h"
#include "luabind.h"

#include <renderer/renderer.h>
#include <renderer/immgfx.h>

namespace app {

	using namespace renderer;

	struct LuaBindRendererFunc {
		static int DrawSprite(lua_State * L) {
			LUA_CHECK_NUM_PARAMS(3);
			LUA_VEC2_PARAM(1, pos);
			LUA_VEC2_PARAM(2, size);
			LUA_VEC4_PARAM(3, col);
			
			Renderer::Ref().DrawSprite(*pos, *size, *col);

			return 0;
		}

		static int BeginScene(lua_State * L) {
			Renderer::Ref().BeginScene();
			return 0;
		}

		static int EndScene(lua_State * L) {
			Renderer::Ref().EndScene();
			return 0;
		}
	};

	LUA_LIB_START(Renderer)
		LUA_LIB_ENTRY("BeginScene", LuaBindRendererFunc::BeginScene)
		LUA_LIB_ENTRY("EndScene", LuaBindRendererFunc::EndScene)
		LUA_LIB_ENTRY("DrawSprite", LuaBindRendererFunc::DrawSprite)
	LUA_LIB_END(Renderer)

	struct LuaBindImmGFXFunc {
		static int Color(lua_State * L) {
			LUA_CHECK_NUM_PARAMS(1);
			LUA_VEC4_PARAM(1, color);
			ImmGFX::Ref().Color(*color);
			return 0;
		}

		static int DrawLine(lua_State * L) {
			LUA_CHECK_NUM_PARAMS(2);
			LUA_VEC2_PARAM(1, a);
			LUA_VEC2_PARAM(2, b);

			ImmGFX::Ref().DrawLine(*a, *b);
			return 0;
		}

		static int DrawCircle(lua_State * L) {
			LUA_CHECK_NUM_PARAMS(2);
			LUA_VEC2_PARAM(1, p);
			LUA_FLOAT_PARAM(2, r);

			ImmGFX::Ref().DrawCircle(*p, r);
			return 0;
		}

		static int DrawCircleWire(lua_State * L) {
			LUA_CHECK_NUM_PARAMS(2);
			LUA_VEC2_PARAM(1, p);
			LUA_FLOAT_PARAM(2, r);

			ImmGFX::Ref().DrawCircleWire(*p, r);
			return 0;
		}

		static int DrawRect(lua_State * L) {
			LUA_CHECK_NUM_PARAMS(2);
			LUA_VEC2_PARAM(1, pos);
			LUA_VEC2_PARAM(2, dimen);

			ImmGFX::Ref().DrawRect(*pos, *dimen);
			return 0;
		}
	};

	LUA_LIB_START(ImmGFX)
		LUA_LIB_ENTRY("Color", LuaBindImmGFXFunc::Color)
		LUA_LIB_ENTRY("DrawLine", LuaBindImmGFXFunc::DrawLine)
		LUA_LIB_ENTRY("DrawCircle", LuaBindImmGFXFunc::DrawCircle)
		LUA_LIB_ENTRY("DrawCircleWire", LuaBindImmGFXFunc::DrawCircleWire)
		LUA_LIB_ENTRY("DrawRect", LuaBindImmGFXFunc::DrawRect)
	LUA_LIB_END(ImmGFX)

	void LuaBindRendererLib(lua_State * L) {
		LUA_REGISTER_LIB(Renderer);
		LUA_REGISTER_LIB(ImmGFX);
	}

}
