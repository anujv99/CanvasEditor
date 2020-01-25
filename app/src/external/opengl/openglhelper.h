
/*
 *	Helper functions
 */

#ifdef __OPENGL__

#ifndef __OPENGL_HELPER_H__
#define __OPENGL_HELPER_H__

#include <glad/glad.h>

#include <graphics/buffer.h>

namespace app { namespace external { namespace opengl {

	struct OpenGLHelper {
		static GLenum GetOpenGLType(graphics::BufferUsage usage);
	};

} } }

#endif //__OPENGL_HELPER_H__

#endif //__OPENGL__
