
/*
 *  Implementation of the graphics texture class interface
 */

#ifdef __OPENGL__

#ifndef __OPENGL_TEXTURE_H__
#define __OPENGL_TEXTURE_H__

#include <graphics/texture.h>

#include "openglhelper.h"

namespace app { namespace external { namespace opengl {

	class OpenGLTexture : public graphics::Texture {
	public:
		OpenGLTexture(Vec2i size, graphics::TextureParams params);
		~OpenGLTexture();
	public:
		virtual void Bind(unsigned int slot) const override;
		virtual void UnBind() const override;
		virtual void SetData(const unsigned char * pixels, size_t size) override;

		virtual Vec2i GetSize() const override { return Vec2i(m_Width, m_Height); };
	public:
		GLuint GetID() const { return m_ID; }
	private:
		GLuint m_ID;
		GLuint m_Width, m_Height;
		GLuint m_BytesPerPixel;
		GLenum m_Format, m_InternalFormat;
		mutable GLint m_LastSlot;
	};

} } }

#endif //__OPENGL_TEXTURE_H__

#endif //__OPENGL__
