#include "openglhelper.h"

#ifdef __OPENGL__

namespace app { namespace external { namespace opengl {

	GLenum OpenGLHelper::GetOpenGLType(graphics::BufferUsage usage) {
		switch (usage) {
		case graphics::BufferUsage::STATIC:			return GL_STATIC_DRAW;
		case graphics::BufferUsage::DYNAMIC:		return GL_DYNAMIC_DRAW;
		default:									return GL_INVALID_ENUM;
		}
	}

} } }

#endif //__OPENGL__