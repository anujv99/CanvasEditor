#include "openglframebuffer.h"

#ifdef __OPENGL__

namespace app {

	namespace graphics {
		utils::StrongHandle<Framebuffer> Framebuffer::Create(Vec2i size, TextureFormat format) {
			return dynamic_cast<Framebuffer *>(new app::external::opengl::OpenGLFramebuffer(size, format));
		}
	}

	namespace external { namespace opengl {

		OpenGLFramebuffer::OpenGLFramebuffer(Vec2i size, graphics::TextureFormat format) : m_Tex(nullptr), m_ID(0u) {
			ASSERTM(size.x > 0 && size.y > 0, "[OpenGL] Invalid size for framebuffer");

			glCreateFramebuffers(1, &m_ID);
			ASSERTM(m_ID > 0, "[OpenGL] Failed to create framebuffer");

			graphics::TextureParams params;
			params.Filtering = graphics::TextureFiltering::NEAREST;
			params.InternalFormat = format;
			params.Wrapping = graphics::TextureWrapping::CLAMP;

			m_Tex = graphics::Texture::Create(size, params);

			GLuint texID = dynamic_cast<OpenGLTexture *>(m_Tex.Get())->GetID();

			glBindFramebuffer(GL_FRAMEBUFFER, m_ID);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texID, 0);

			ASSERTM(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "[OpenGL] Failed to create framebuffer");

			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		OpenGLFramebuffer::~OpenGLFramebuffer() {
			glDeleteFramebuffers(1, &m_ID);
		}

		void OpenGLFramebuffer::Bind() {
			glBindFramebuffer(GL_FRAMEBUFFER, m_ID);
		}

		void OpenGLFramebuffer::UnBind() {
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		void OpenGLFramebuffer::Clear() {
			glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
			glClear(GL_COLOR_BUFFER_BIT);
		}

		void OpenGLFramebuffer::Clear(Vec4 color) {
			glClearColor(color.x, color.y, color.z, color.w);
			glClear(GL_COLOR_BUFFER_BIT);
		}

	} }

}

#endif //__OPENGL__
