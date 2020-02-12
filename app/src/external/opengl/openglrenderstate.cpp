#include "openglrenderstate.h"

#ifdef __OPENGL__

namespace app {

	namespace graphics {
		RenderState * RenderState::Create() {
			return dynamic_cast<RenderState *>(new app::external::opengl::OpenGLRenderState());
		}
	}

	namespace external { namespace opengl {

		OpenGLRenderState::OpenGLRenderState() : m_Prim(), m_Viewport(), m_BlendFunc(), m_OpenGLPrim(GL_INVALID_ENUM), m_LineThickness(1.0f) {}

		OpenGLRenderState::~OpenGLRenderState() {}

		void OpenGLRenderState::SetTopology(graphics::Topology topology) {
			m_OpenGLPrim = OpenGLHelper::GetOpenGLType(topology);
			m_Prim = topology;
		}

		void OpenGLRenderState::SetViewport(graphics::Viewport viewport) {
			m_Viewport = viewport;
			glViewport(viewport.TopLeftX, viewport.TopLeftY, viewport.Width, viewport.Height);
		}

		void OpenGLRenderState::SetBlendFunction(graphics::BlendFunction func) {
			if (!func.EnableBlending) {
				glDisable(GL_BLEND);
			} else {
				glEnable(GL_BLEND);
				glBlendFuncSeparate(
					OpenGLHelper::GetOpenGLType(func.SrcColor),
					OpenGLHelper::GetOpenGLType(func.DestColor),
					OpenGLHelper::GetOpenGLType(func.SrcAlpha),
					OpenGLHelper::GetOpenGLType(func.DestAlpha));
				glBlendEquationSeparate(OpenGLHelper::GetOpenGLType(func.ColorOperation), OpenGLHelper::GetOpenGLType(func.AlphaOperation));
			}

			m_BlendFunc = func;
		}

		void OpenGLRenderState::SetLineThickness(float thickness) {
			glLineWidth(thickness);
			m_LineThickness = thickness;
		}

	} }

}

#endif //__OPENGL__