#include "paintrenderer.h"

#include <core/window.h>
#include <renderer/renderer.h>

#include <vm/lua.hpp>
#include <vm/luabind/luabind.h>

using namespace app;
using namespace core;
using namespace utils;
using namespace graphics;
using namespace renderer;

namespace paint {

	static constexpr const char VERTEX_SHADER[] = R"(
		#version 450 core
			
		layout (location = 0) in vec2 aPos;
		
		void main() {
			gl_Position = vec4(aPos.xy, 0.0f, 1.0f);
		}
	)";

	static constexpr const char FRAGMENT_SHADER[] = R"(
		#version 450 core

		out vec4 FragColor;

		uniform vec2 StartPos;
		uniform vec2 EndPos;
		uniform float BrushSize;
		uniform vec4 BrushColor;

		float DistanceFromLine(vec2 v, vec2 w, vec2 p) {
			float l2 = pow(length(v - w), 2);
			if (l2 == 0.0f) { return length(p - v); }

			float t = max(0, min(1, dot(p - v, w - v) / l2));
			vec2 projection = v + t * (w - v);
			return length(p - projection);
		}

		void main() {
			float dist = DistanceFromLine(StartPos, EndPos, gl_FragCoord.xy);
			if (dist <= BrushSize) {
				FragColor = BrushColor;
			} else {
				discard;
			}
		}
	)";

	PaintRenderer::PaintRenderer() : m_Shader(nullptr), m_Framebuffer(nullptr) {
		m_Shader = ShaderProgram::Create(VertexShader::Create(VERTEX_SHADER), FragmentShader::Create(FRAGMENT_SHADER));

		float vertices[] = {
			-1.0,  1.0,
			 1.0,  1.0,
			 1.0, -1.0,
			-1.0,  1.0,
			 1.0, -1.0,
			-1.0, -1.0,
		};

		StrongHandle<VertexBuffer> buffer = VertexBuffer::Create(vertices, sizeof(vertices), BufferUsage::STATIC);

		StrongHandle<BufferLayout> layout = BufferLayout::Create();
		layout->BeginEntries();
		layout->AddEntry(DataType::Float2, 0, "POSITION", false);
		layout->EndEntries();

		buffer->SetBufferLayout(layout);

		m_VertexArray = VertexArray::Create();
		m_VertexArray->AddVertexBuffer(buffer);

		Vec2i winSize = Vec2i(Window::Ref().GetWidth(), Window::Ref().GetHeight());

		m_Framebuffer = Framebuffer::Create(winSize, TextureFormat::RGBA);
	}

	PaintRenderer::~PaintRenderer() {}

	void PaintRenderer::Clear() {
		m_Framebuffer->Bind();
		m_Framebuffer->Clear();
		m_Framebuffer->UnBind();
	}

	void PaintRenderer::DrawLine(Vec2 pos1, Vec2 pos2, float brushSize, Vec4 color) {
		m_Framebuffer->Bind();
		m_Shader->Bind();
		m_VertexArray->Bind();

		m_Shader->SetUniformVec2(m_Shader->GetUniformLocation("StartPos"), pos1);
		m_Shader->SetUniformVec2(m_Shader->GetUniformLocation("EndPos"), pos2);
		m_Shader->SetUniformFloat(m_Shader->GetUniformLocation("BrushSize"), brushSize);
		m_Shader->SetUniformVec4(m_Shader->GetUniformLocation("BrushColor"), color);

		m_VertexArray->Draw(6u);

		m_Framebuffer->UnBind();
	}

	void PaintRenderer::Render() {
		Renderer::Ref().PassFramebuffer(m_Framebuffer, nullptr);
	}

	void PaintRenderer::BindLuaLib(lua_State * L) {
		struct LuaBindLibFunc {
			static int DrawLine(lua_State * L) {
				LUA_CHECK_NUM_PARAMS(4);
				LUA_VEC2_PARAM(1, startPos);
				LUA_VEC2_PARAM(2, endPos);
				LUA_FLOAT_PARAM(3, brushSize);
				LUA_VEC4_PARAM(4, color);

				PaintRenderer::Ref().DrawLine(*startPos, *endPos, brushSize, *color);
				return 0;
			}

			static int Render(lua_State * L) {
				LUA_CHECK_NUM_PARAMS(0);
				PaintRenderer::Ref().Render();
				return 0;
			}

			static int Clear(lua_State * L) {
				LUA_CHECK_NUM_PARAMS(0);
				PaintRenderer::Ref().Clear();
				return 0;
			}
		};

		lua_newtable(L);

		lua_pushstring(L, "DrawLine");
		lua_pushcfunction(L, LuaBindLibFunc::DrawLine);
		lua_settable(L, -3);

		lua_pushstring(L, "Render");
		lua_pushcfunction(L, LuaBindLibFunc::Render);
		lua_settable(L, -3);

		lua_pushstring(L, "Clear");
		lua_pushcfunction(L, LuaBindLibFunc::Clear);
		lua_settable(L, -3);

		lua_setglobal(L, "PaintRenderer");
	}

}