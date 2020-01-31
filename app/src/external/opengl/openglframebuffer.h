
#ifdef __OPENGL__

#ifndef __OPENGL_FRAMEBUFFER_H__
#define __OPENGL_FRAMEBUFFER_H__

#include <graphics/framebuffer.h>

#include "opengltexture.h"
#include "openglhelper.h"

namespace app { namespace external { namespace opengl {

	class OpenGLFramebuffer : public graphics::Framebuffer {
	public:
		OpenGLFramebuffer(Vec2i size, graphics::TextureFormat format);
		~OpenGLFramebuffer();
	public:
		virtual void Bind() override;
		virtual void UnBind() override;
		virtual void Clear() override;
		virtual void Clear(Vec4 color) override;

		inline virtual Vec2i GetSize() const override { return m_Tex->GetSize(); }
		inline virtual utils::StrongHandle<graphics::Texture> GetTexture() override { return m_Tex; }
	private:
		utils::StrongHandle<graphics::Texture> m_Tex;
		GLuint m_ID;
	};

} } }

#endif //__OPENGL_FRAMEBUFFER_H__

#endif //__OPENGL__
