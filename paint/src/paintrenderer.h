
#ifndef __PAINT_RENDERER_H__
#define __PAINT_RENDERER_H__

#include <utils/singleton.h>
#include <math/math.h>

#include <graphics/shader.h>
#include <graphics/framebuffer.h>
#include <graphics/vertexarray.h>
#include <graphics/buffer.h>

struct lua_State;

namespace paint {

	using namespace prevmath;

	class PaintRenderer : public app::utils::Singleton<PaintRenderer> {
		friend class app::utils::Singleton<PaintRenderer>;
	private:
		PaintRenderer();
		~PaintRenderer();
	public:
		void Clear();
		void DrawLine(Vec2 pos1, Vec2 pos2, float brushSize, Vec4 color);
		void DrawCircle(Vec2 pos, float radius, Vec4 color);
		void DrawRect(Vec2 pos1, Vec2 pos2, Vec4 color);
		void Render();

		void BindLuaLib(lua_State * L);
	private:
		app::utils::StrongHandle<app::graphics::ShaderProgram> m_Shader;
		app::utils::StrongHandle<app::graphics::VertexArray> m_VertexArray;
		app::utils::StrongHandle<app::graphics::Framebuffer> m_Framebuffer;
	};

}

#endif //__PAINT_RENDERER_H__
