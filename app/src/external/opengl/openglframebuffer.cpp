#include "openglframebuffer.h"

#ifdef __OPENGL__

namespace app {

	namespace graphics {
		utils::StrongHandle<Framebuffer> Framebuffer::Create(Vec2i size, TextureFormat format, FBOFlags flags) {
			return dynamic_cast<Framebuffer *>(new app::external::opengl::OpenGLFramebuffer(size, format, flags));
		}
	}

	namespace external { namespace opengl {

		OpenGLFramebuffer::OpenGLFramebuffer(Vec2i size, graphics::TextureFormat format, graphics::FBOFlags flags) :
			m_Tex(nullptr), m_Size(size), m_ID(0u), m_IsMultisampled(false), m_MSAATexID(0u) {
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

			GLuint numSamples = 0u;

			if (flags & graphics::FBOFlags::MSAA_16X)
				numSamples = 16;
			else if (flags & graphics::FBOFlags::MSAA_8X)
				numSamples = 8;
			else if (flags & graphics::FBOFlags::MSAA_4X)
				numSamples = 4;
			else if (flags & graphics::FBOFlags::MSAA_2X)
				numSamples = 2;
			else
				return;

			m_IsMultisampled = true;

			glCreateTextures(GL_TEXTURE_2D_MULTISAMPLE, 1, &m_MSAATexID);
			ASSERTM(m_MSAATexID > 0u, "[OpenGL] Failed to create multisampled texture");

			glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_MSAATexID);
			glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, numSamples, OpenGLHelper::GetTextureInternalFormat(format), size.x, size.y, false);

			glGenFramebuffers(1, &m_MSAAFbo);
			ASSERTM(m_ID > 0, "[OpenGL] Failed to create multisampled framebuffer");
			glBindFramebuffer(GL_FRAMEBUFFER, m_MSAAFbo);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, m_MSAATexID, 0);

			ASSERTM(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "[OpenGL] Failed to create multisampled framebuffer");

			glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0u);
			glBindFramebuffer(GL_FRAMEBUFFER, 0u);
		}

		OpenGLFramebuffer::~OpenGLFramebuffer() {
			glDeleteFramebuffers(1, &m_ID);
		}

		void OpenGLFramebuffer::Bind() {
			if (m_IsMultisampled)
				glBindFramebuffer(GL_FRAMEBUFFER, m_MSAAFbo);
			else
				glBindFramebuffer(GL_FRAMEBUFFER, m_ID);
		}

		void OpenGLFramebuffer::UnBind() {
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		void OpenGLFramebuffer::Resolve() {
			if (!m_IsMultisampled) {
				LOG_WARN("[OpenGL] Resolve is only required while using multisampled fbo");
				return;
			}
			// glBlitNamedFramebuffer not working on some system
			//glBlitNamedFramebuffer(m_MSAAFbo, m_ID, 0, 0, m_Size.x, m_Size.y, 0, 0, m_Size.x, m_Size.y, GL_COLOR_BUFFER_BIT, GL_NEAREST);
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_ID);
			glBindFramebuffer(GL_READ_FRAMEBUFFER, m_MSAAFbo);
			glBlitFramebuffer(0, 0, m_Size.x, m_Size.y, 0, 0, m_Size.x, m_Size.y, GL_COLOR_BUFFER_BIT, GL_NEAREST);
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
			glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
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
