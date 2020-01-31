
/*
 *	Helper functions
 */

#ifdef __OPENGL__

#ifndef __OPENGL_HELPER_H__
#define __OPENGL_HELPER_H__

#include <glad/glad.h>

#include <graphics/buffer.h>
#include <graphics/vertexarray.h>
#include <graphics/renderstate.h>
#include <graphics/bufferlayout.h>
#include <graphics/texture.h>

namespace app { namespace external { namespace opengl {

	struct OpenGLHelper {
		static GLenum GetOpenGLType(graphics::BufferUsage usage);
		static GLenum GetOpenGLType(graphics::DataType type);
		static GLenum GetOpenGLType(graphics::BlendColorOption option);
		static GLenum GetOpenGLType(graphics::BlendAlphaOption option);
		static GLenum GetOpenGLType(graphics::BlendOperation operation);
		static GLenum GetOpenGLType(graphics::Topology topology);
		static GLenum GetOpenGLType(graphics::TextureFormat format);
		static GLenum GetOpenGLType(graphics::TextureFiltering filter);
		static GLenum GetOpenGLType(graphics::TextureWrapping wrap);


		static GLenum GetTextureInternalFormat(graphics::TextureFormat format);
		static unsigned int GetNumBytes(graphics::TextureFormat format);
	};

} } }

#endif //__OPENGL_HELPER_H__

#endif //__OPENGL__
