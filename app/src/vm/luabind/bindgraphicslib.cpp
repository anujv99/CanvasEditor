#include "bindgraphicslib.h"
#include "luabind.h"

#include "graphics/renderstate.h"
#include "graphics/shader.h"
#include "graphics/buffer.h"
#include "graphics/vertexarray.h"
#include "graphics/framebuffer.h"

#include "renderer/renderer.h"

namespace app {

	using namespace graphics;

	// ------------------------------------ RENDER STATE ------------------------------------

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

	// ------------------------------------ SHADERS ------------------------------------

	struct LuaBindShaderFunc {
		static int Bind(lua_State * L) {
			LUA_CHECK_NUM_PARAMS(1);
			LUA_HANDLED_OBJECT_PARAM(1, ShaderProgram, sp);
			
			sp->Bind();
			return 0;
		}

		static int SetUniformVec2(lua_State * L) {
			LUA_CHECK_NUM_PARAMS(3);
			LUA_HANDLED_OBJECT_PARAM(1, ShaderProgram, sp);
			LUA_STRING_PARAM(2, un);
			LUA_VEC2_PARAM(3, val);
			sp->SetUniformVec2(sp->GetUniformLocation(un), *val);
			return 0;
		}

		static int SetUniformFloat(lua_State * L) {
			LUA_CHECK_NUM_PARAMS(3);
			LUA_HANDLED_OBJECT_PARAM(1, ShaderProgram, sp);
			LUA_STRING_PARAM(2, un);
			LUA_FLOAT_PARAM(3, val);
			sp->SetUniformFloat(sp->GetUniformLocation(un), val);
			return 0;
		}

		static int Create(lua_State * L) {
			LUA_CHECK_NUM_PARAMS(2);
			LUA_STRING_PARAM(1, vsc);
			LUA_STRING_PARAM(2, fsc);

			utils::StrongHandle<VertexShader> vs = VertexShader::Create(vsc);
			utils::StrongHandle<FragmentShader> fs = FragmentShader::Create(fsc);
			utils::StrongHandle<ShaderProgram> sp = ShaderProgram::Create(vs, fs);

			lua_newtable(L);

			LUA_SET_HANDLED_OBJECT_HANDLE(sp);
			LUA_SET_CUSTOM_OBJECT_TYPE(ShaderProgram);

			lua_pushstring(L, "Bind");
			lua_pushcfunction(L, Bind);
			lua_settable(L, -3);

			lua_pushstring(L, "SetUniformVec2");
			lua_pushcfunction(L, SetUniformVec2);
			lua_settable(L, -3);

			lua_pushstring(L, "SetUniformFloat");
			lua_pushcfunction(L, SetUniformFloat);
			lua_settable(L, -3);

			luaL_getmetatable(L, "LuaShaderMetatable");
			if (lua_type(L, -1) != LUA_TTABLE) { LUA_LOG("LuaShaderMetatable not found"); return 0; }
			lua_setmetatable(L, -2);
			
			return 1;
		}
	};

	LUA_LIB_START(Shader)
		LUA_LIB_ENTRY("Create", LuaBindShaderFunc::Create)
	LUA_LIB_END(Shader)

	LUA_METATABLE_START(Shader)
		LUA_GENERATE_HANDLED_OBJECT_DESTRUCTOR(ShaderProgram)
	LUA_METATBLE_END(Shader)

	void BindShader(lua_State * L) {
		luaL_newmetatable(L, "LuaShaderMetatable");
		for (size_t i = 0; i < std::size(LuaShaderMetatableFunctions); i++) {
			lua_pushstring(L, LuaShaderMetatableFunctions[i].Name);
			lua_pushcfunction(L, LuaShaderMetatableFunctions[i].Func);
			lua_settable(L, -3);
		}
		lua_pop(L, 1);

		LUA_REGISTER_LIB(Shader);
	}

	// ------------------------------------ BUFFER ------------------------------------

	struct LuaBindVertexBufferFunc {
		static int SetBufferLayout(lua_State * L) {
			LUA_CHECK_NUM_PARAMS(2);
			LUA_HANDLED_OBJECT_PARAM(1, VertexBuffer, vb);
			LUA_HANDLED_OBJECT_PARAM(2, BufferLayout, bl);

			vb->SetBufferLayout(bl);
			return 0;
		}

		static int Create(lua_State * L) {
			LUA_CHECK_NUM_PARAMS(1);
			LUA_CHECK_TABLE_PARAM(1);

			lua_len(L, 1);
			int size = lua_tointeger(L, -1);
			lua_pop(L, 1);

			if (size <= 0) { LUA_DEBUG_LOG("Invalid arrray size"); return 0; }

			float * arr = new float[size];

			for (int i = 1; i <= size; i++) {
				lua_rawgeti(L, 1, i);
				//if (lua_type(L, -1) != LUA_TNUMBER) { LUA_DEBUG_LOG("Expected float at index %d of param 1", i); delete[] arr; return 0; }
				arr[i - 1] = lua_tonumber(L, -1);
				lua_pop(L, 1);
			}

			utils::StrongHandle<VertexBuffer> vb = VertexBuffer::Create(arr, sizeof(float) * size, BufferUsage::STATIC);

			delete[] arr;

			lua_newtable(L);

			LUA_SET_HANDLED_OBJECT_HANDLE(vb);
			LUA_SET_CUSTOM_OBJECT_TYPE(VertexBuffer);

			lua_pushstring(L, "SetBufferLayout");
			lua_pushcfunction(L, SetBufferLayout);
			lua_settable(L, -3);

			luaL_getmetatable(L, "LuaVertexBufferMetatable");
			if (lua_type(L, -1) != LUA_TTABLE) { LUA_LOG("LuaShaderMetatable not found"); return 0; }
			lua_setmetatable(L, -2);

			return 1;
		}
	};

	LUA_LIB_START(VertexBuffer)
		LUA_LIB_ENTRY("Create", LuaBindVertexBufferFunc::Create)
	LUA_LIB_END(VertexBuffer)

	LUA_METATABLE_START(VertexBuffer)
		LUA_GENERATE_HANDLED_OBJECT_DESTRUCTOR(VertexBuffer)
	LUA_METATBLE_END(VertexBuffer)

	void BindVertexBuffer(lua_State * L) {
		luaL_newmetatable(L, "LuaVertexBufferMetatable");
		for (size_t i = 0; i < std::size(LuaVertexBufferMetatableFunctions); i++) {
			lua_pushstring(L, LuaVertexBufferMetatableFunctions[i].Name);
			lua_pushcfunction(L, LuaVertexBufferMetatableFunctions[i].Func);
			lua_settable(L, -3);
		}
		lua_pop(L, 1);

		LUA_REGISTER_LIB(VertexBuffer);
	}

	// ------------------------------------ VERTEX ARRAY ------------------------------------

	struct LuaBindVertexArrayFunc {
		static int AddVertexBuffer(lua_State * L) {
			LUA_CHECK_NUM_PARAMS(2);
			LUA_HANDLED_OBJECT_PARAM(1, VertexArray, va);
			LUA_HANDLED_OBJECT_PARAM(2, VertexBuffer, vb);
			va->AddVertexBuffer(vb);
			return 0;
		}

		static int Bind(lua_State * L) {
			LUA_CHECK_NUM_PARAMS(1);
			LUA_HANDLED_OBJECT_PARAM(1, VertexArray, va);
			va->Bind();
			return 0;
		}

		static int UnBind(lua_State * L) {
			LUA_CHECK_NUM_PARAMS(1);
			LUA_HANDLED_OBJECT_PARAM(1, VertexArray, va);
			va->UnBind();
			return 0;
		}

		static int Draw(lua_State * L) {
			LUA_CHECK_NUM_PARAMS(2);
			LUA_HANDLED_OBJECT_PARAM(1, VertexArray, va);
			LUA_INT_PARAM(2, numVertices);
			va->Draw(numVertices);
			return 0;
		}

		static int Create(lua_State * L) {
			LUA_CHECK_NUM_PARAMS(0);

			utils::StrongHandle<VertexArray> va = VertexArray::Create();

			lua_newtable(L);

			LUA_SET_HANDLED_OBJECT_HANDLE(va);
			LUA_SET_CUSTOM_OBJECT_TYPE(VertexArray);

			lua_pushstring(L, "AddVertexBuffer");
			lua_pushcfunction(L, AddVertexBuffer);
			lua_settable(L, -3);

			lua_pushstring(L, "Bind");
			lua_pushcfunction(L, Bind);
			lua_settable(L, -3);

			lua_pushstring(L, "UnBind");
			lua_pushcfunction(L, UnBind);
			lua_settable(L, -3);

			lua_pushstring(L, "Draw");
			lua_pushcfunction(L, Draw);
			lua_settable(L, -3);

			luaL_getmetatable(L, "LuaVertexArrayMetatable");
			if (lua_type(L, -1) != LUA_TTABLE) { LUA_LOG("LuaShaderMetatable not found"); return 0; }
			lua_setmetatable(L, -2);

			return 1;
		}
	};

	LUA_LIB_START(VertexArray)
		LUA_LIB_ENTRY("Create", LuaBindVertexArrayFunc::Create)
	LUA_LIB_END(VertexArray)

	LUA_METATABLE_START(VertexArray)
		LUA_GENERATE_HANDLED_OBJECT_DESTRUCTOR(VertexArray)
	LUA_METATBLE_END(VertexArray)

	void BindVertexArray(lua_State * L) {
		luaL_newmetatable(L, "LuaVertexArrayMetatable");
		for (size_t i = 0; i < std::size(LuaVertexArrayMetatableFunctions); i++) {
			lua_pushstring(L, LuaVertexArrayMetatableFunctions[i].Name);
			lua_pushcfunction(L, LuaVertexArrayMetatableFunctions[i].Func);
			lua_settable(L, -3);
		}
		lua_pop(L, 1);

		LUA_REGISTER_LIB(VertexArray);
	}

	// ------------------------------------ BUFFER LAYOUT ------------------------------------

	struct LuaBindBufferLayoutFunc {

		static int BeginEntries(lua_State * L) {
			LUA_CHECK_NUM_PARAMS(1);
			LUA_HANDLED_OBJECT_PARAM(1, BufferLayout, bl);
			bl->BeginEntries();
			return 0;
		}

		static int EndEntries(lua_State * L) {
			LUA_CHECK_NUM_PARAMS(1);
			LUA_HANDLED_OBJECT_PARAM(1, BufferLayout, bl);
			bl->EndEntries();
			return 0;
		}

		static int AddEntry(lua_State * L) {
			LUA_CHECK_NUM_PARAMS(4);
			LUA_HANDLED_OBJECT_PARAM(1, BufferLayout, bl);
			LUA_INT_PARAM(2, type);
			LUA_INT_PARAM(3, offset);
			LUA_BOOL_PARAM(4, normalize);
			bl->AddEntry((DataType)type, offset, "RIP", normalize);
			return 0;
		}

		static int Create(lua_State * L) {
			LUA_CHECK_NUM_PARAMS(0);
			utils::StrongHandle<BufferLayout> bl = BufferLayout::Create();

			lua_newtable(L);
			LUA_SET_HANDLED_OBJECT_HANDLE(bl);
			LUA_SET_CUSTOM_OBJECT_TYPE(BufferLayout);

			lua_pushstring(L, "BeginEntries");
			lua_pushcfunction(L, BeginEntries);
			lua_settable(L, -3);

			lua_pushstring(L, "EndEntries");
			lua_pushcfunction(L, EndEntries);
			lua_settable(L, -3);

			lua_pushstring(L, "AddEntry");
			lua_pushcfunction(L, AddEntry);
			lua_settable(L, -3);

			luaL_getmetatable(L, "LuaBufferLayoutMetatable");
			if (lua_type(L, -1) != LUA_TTABLE) { LUA_LOG("LuaBufferLayoutMetatable not found"); return 0; }
			lua_setmetatable(L, -2);

			return 1;
		}
	};

	LUA_LIB_START(BufferLayout)
		LUA_LIB_ENTRY("Create", LuaBindBufferLayoutFunc::Create)
	LUA_LIB_END(BufferLayout)

	LUA_METATABLE_START(BufferLayout)
		LUA_GENERATE_HANDLED_OBJECT_DESTRUCTOR(BufferLayout)
	LUA_METATBLE_END(BufferLayout)

#define SET_DATA_TYPE(D) lua_pushstring(L, #D); lua_pushinteger(L, (int)DataType::D); lua_settable(L, -3)

	void BindBufferLayout(lua_State * L) {
		lua_newtable(L);

		SET_DATA_TYPE(Int);
		SET_DATA_TYPE(Int2);
		SET_DATA_TYPE(Int3);
		SET_DATA_TYPE(Int4);
		SET_DATA_TYPE(UInt);
		SET_DATA_TYPE(UInt2);
		SET_DATA_TYPE(UInt3);
		SET_DATA_TYPE(UInt4);
		SET_DATA_TYPE(Float);
		SET_DATA_TYPE(Float2);
		SET_DATA_TYPE(Float3);
		SET_DATA_TYPE(Float4);
		SET_DATA_TYPE(Bool);
		SET_DATA_TYPE(UByte);
		SET_DATA_TYPE(UByte2);
		SET_DATA_TYPE(UByte3);
		SET_DATA_TYPE(UByte4);
		SET_DATA_TYPE(Byte);
		SET_DATA_TYPE(Byte2);
		SET_DATA_TYPE(Byte3);
		SET_DATA_TYPE(Byte4);
		
		lua_setglobal(L, "DataType");

		luaL_newmetatable(L, "LuaBufferLayoutMetatable");
		for (size_t i = 0; i < std::size(LuaBufferLayoutMetatableFunctions); i++) {
			lua_pushstring(L, LuaBufferLayoutMetatableFunctions[i].Name);
			lua_pushcfunction(L, LuaBufferLayoutMetatableFunctions[i].Func);
			lua_settable(L, -3);
		}
		lua_pop(L, 1);

		LUA_REGISTER_LIB(BufferLayout);
	}

	// ------------------------------------ FRAMEBUFFER ------------------------------------

	struct LuaBindFramebufferFunc {
		static int Bind(lua_State * L) {
			LUA_CHECK_NUM_PARAMS(1);
			LUA_HANDLED_OBJECT_PARAM(1, Framebuffer, fbo);
			fbo->Bind();
			return 0;
		}

		static int UnBind(lua_State * L) {
			LUA_CHECK_NUM_PARAMS(1);
			LUA_HANDLED_OBJECT_PARAM(1, Framebuffer, fbo);
			fbo->UnBind();
			return 0;
		}

		static int Clear(lua_State * L) {
			LUA_CHECK_NUM_PARAMS(1);
			LUA_HANDLED_OBJECT_PARAM(1, Framebuffer, fbo);
			fbo->Clear();
			return 0;
		}

		static int Resolve(lua_State * L) {
			LUA_CHECK_NUM_PARAMS(1);
			LUA_HANDLED_OBJECT_PARAM(1, Framebuffer, fbo);
			fbo->Resolve();
			return 0;
		}

		static int DrawToScreen(lua_State * L) {
			LUA_CHECK_NUM_PARAMS(1);
			LUA_HANDLED_OBJECT_PARAM(1, Framebuffer, fbo);
			renderer::Renderer::Ref().PassFramebuffer(fbo, nullptr);
			return 0;
		}

		static int Create(lua_State * L) {
			LUA_CHECK_NUM_PARAMS(1);
			LUA_VEC2_PARAM(1, size);
			
			utils::StrongHandle<Framebuffer> fbo = Framebuffer::Create(ToVec2i(*size), TextureFormat::RGBA, FBOFlags::MSAA_16X);
			
			lua_newtable(L);
			LUA_SET_HANDLED_OBJECT_HANDLE(fbo);
			LUA_SET_CUSTOM_OBJECT_TYPE(Framebuffer);

			lua_pushstring(L, "Bind");
			lua_pushcfunction(L, Bind);
			lua_settable(L, -3);

			lua_pushstring(L, "UnBind");
			lua_pushcfunction(L, UnBind);
			lua_settable(L, -3);

			lua_pushstring(L, "Clear");
			lua_pushcfunction(L, Clear);
			lua_settable(L, -3);

			lua_pushstring(L, "Resolve");
			lua_pushcfunction(L, Resolve);
			lua_settable(L, -3);

			lua_pushstring(L, "DrawToScreen");
			lua_pushcfunction(L, DrawToScreen);
			lua_settable(L, -3);

			luaL_getmetatable(L, "LuaFramebufferMetatable");
			if (lua_type(L, -1) != LUA_TTABLE) { LUA_LOG("LuaFramebufferMetatable not found"); return 0; }
			lua_setmetatable(L, -2);

			return 1;
		}
	};

	LUA_LIB_START(Framebuffer)
		LUA_LIB_ENTRY("Create", LuaBindFramebufferFunc::Create)
	LUA_LIB_END(Framebuffer)

	LUA_METATABLE_START(Framebuffer)
		LUA_GENERATE_HANDLED_OBJECT_DESTRUCTOR(Framebuffer)
	LUA_METATBLE_END(Framebuffer)

	void BindFramebuffer(lua_State * L) {
		luaL_newmetatable(L, "LuaFramebufferMetatable");
		for (size_t i = 0; i < std::size(LuaFramebufferMetatableFunctions); i++) {
			lua_pushstring(L, LuaFramebufferMetatableFunctions[i].Name);
			lua_pushcfunction(L, LuaFramebufferMetatableFunctions[i].Func);
			lua_settable(L, -3);
		}
		lua_pop(L, 1);

		LUA_REGISTER_LIB(Framebuffer);
	}

	void LuaBindGraphicsLib(lua_State * L) {
		BindShader(L);
		BindVertexBuffer(L);
		BindVertexArray(L);
		BindBufferLayout(L);
		BindFramebuffer(L);
	}

}


