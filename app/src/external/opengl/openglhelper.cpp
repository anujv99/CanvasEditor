#include "openglhelper.h"

#ifdef __OPENGL__

namespace app { namespace external { namespace opengl {

	GLenum OpenGLHelper::GetOpenGLType(graphics::BufferUsage usage) {
		switch (usage) {
		case graphics::BufferUsage::STATIC:			return GL_STATIC_DRAW;
		case graphics::BufferUsage::DYNAMIC:		return GL_DYNAMIC_DRAW;
		}

		ASSERTM(false, "[OpenGL] Inavlid buffer usage");
		return GL_INVALID_ENUM;
	}

	GLenum OpenGLHelper::GetOpenGLType(graphics::DataType type) {
		switch (type) {
		case graphics::DataType::Int:		return GL_INT;
		case graphics::DataType::Int2:		return GL_INT;
		case graphics::DataType::Int3:		return GL_INT;
		case graphics::DataType::Int4:		return GL_INT;
		case graphics::DataType::UInt:		return GL_UNSIGNED_INT;
		case graphics::DataType::UInt2:		return GL_UNSIGNED_INT;
		case graphics::DataType::UInt3:		return GL_UNSIGNED_INT;
		case graphics::DataType::UInt4:		return GL_UNSIGNED_INT;
		case graphics::DataType::Float:		return GL_FLOAT;
		case graphics::DataType::Float2:	return GL_FLOAT;
		case graphics::DataType::Float3:	return GL_FLOAT;
		case graphics::DataType::Float4:	return GL_FLOAT;
		case graphics::DataType::Bool:		return GL_BOOL;
		case graphics::DataType::UByte:		return GL_UNSIGNED_BYTE;
		case graphics::DataType::UByte2:	return GL_UNSIGNED_BYTE;
		case graphics::DataType::UByte4:	return GL_UNSIGNED_BYTE;
		case graphics::DataType::Byte:		return GL_BYTE;
		case graphics::DataType::Byte2:		return GL_BYTE;
		case graphics::DataType::Byte4:		return GL_BYTE;
		}

		ASSERTM(false, "[OpenGL] Inavlid data type");
		return GL_INVALID_ENUM;
	}

	GLenum OpenGLHelper::GetOpenGLType(graphics::BlendColorOption option) {
		switch (option) {
		case graphics::BlendColorOption::ZERO:				return GL_ZERO;
		case graphics::BlendColorOption::ONE:				return GL_ONE;
		case graphics::BlendColorOption::SRC_COLOR:			return GL_SRC_COLOR;
		case graphics::BlendColorOption::INV_SRC_COLOR:		return GL_ONE_MINUS_SRC_COLOR;
		case graphics::BlendColorOption::DEST_COLOR:		return GL_DST_COLOR;
		case graphics::BlendColorOption::INV_DEST_COLOR:	return GL_ONE_MINUS_DST_COLOR;
		case graphics::BlendColorOption::SRC_ALPHA:			return GL_SRC_ALPHA;
		case graphics::BlendColorOption::INV_SRC_ALPHA:		return GL_ONE_MINUS_SRC_ALPHA;
		case graphics::BlendColorOption::DEST_ALPHA:		return GL_DST_ALPHA;
		case graphics::BlendColorOption::INV_DEST_ALPHA:	return GL_ONE_MINUS_DST_ALPHA;
		}

		ASSERTM(false, "[OpenGL] Invalid BlendColorOption");
		return GL_INVALID_ENUM;
	}

	GLenum OpenGLHelper::GetOpenGLType(graphics::BlendAlphaOption option) {
		switch (option) {
		case graphics::BlendAlphaOption::ZERO:				return GL_ZERO;
		case graphics::BlendAlphaOption::ONE:				return GL_ONE;
		case graphics::BlendAlphaOption::SRC_ALPHA:			return GL_SRC_ALPHA;
		case graphics::BlendAlphaOption::INV_SRC_ALPHA:		return GL_ONE_MINUS_SRC_ALPHA;
		case graphics::BlendAlphaOption::DEST_ALPHA:		return GL_DST_ALPHA;
		case graphics::BlendAlphaOption::INV_DEST_ALPHA:	return GL_ONE_MINUS_DST_ALPHA;
		}

		ASSERTM(false, "[OpenGL] Invalid BlendAlphaOption");
		return GL_INVALID_ENUM;
	}

	GLenum OpenGLHelper::GetOpenGLType(graphics::BlendOperation operation) {
		switch (operation) {
		case graphics::BlendOperation::ADD:				return GL_FUNC_ADD;
		case graphics::BlendOperation::SUBTRACT:		return GL_FUNC_SUBTRACT;
		case graphics::BlendOperation::REV_SUBTRACT:	return GL_FUNC_REVERSE_SUBTRACT;
		case graphics::BlendOperation::MIN:				return GL_MIN;
		case graphics::BlendOperation::MAX:				return GL_MAX;
		}

		ASSERTM(false, "[OpenGL] Invalid BlendOperation");
		return GL_INVALID_ENUM;
	}

	GLenum OpenGLHelper::GetOpenGLType(graphics::Topology topology) {
		switch (topology) {
		case graphics::Topology::POINT:					return GL_POINTS;
		case graphics::Topology::LINE:					return GL_LINES;
		case graphics::Topology::LINE_ADJ:				return GL_LINES_ADJACENCY;
		case graphics::Topology::LINE_STRIP:			return GL_LINE_STRIP;
		case graphics::Topology::LINE_STRIP_ADJ:		return GL_LINE_STRIP_ADJACENCY;
		case graphics::Topology::TRIANGLE:				return GL_TRIANGLES;
		case graphics::Topology::TRIANGLE_ADJ:			return GL_TRIANGLES_ADJACENCY;
		case graphics::Topology::TRIANGLE_STRIP:		return GL_TRIANGLE_STRIP;
		case graphics::Topology::TRIANGLE_STRIP_ADJ:	return GL_TRIANGLE_STRIP_ADJACENCY;
		}

		ASSERTM(false, "[OpenGL] Invalid Primitive Topology");
		return GL_INVALID_ENUM;
	}

	GLenum OpenGLHelper::GetOpenGLType(graphics::TextureFormat format) {
		switch (format) {
		case graphics::TextureFormat::R:	return GL_RED;
		case graphics::TextureFormat::RG:	return GL_RG;
		case graphics::TextureFormat::RGB:	return GL_RGB;
		case graphics::TextureFormat::RGBA:	return GL_RGBA;
		}

		ASSERTM(false, "[OpenGL] Invalid Texture Format");
		return GL_INVALID_ENUM;
	}

	GLenum OpenGLHelper::GetOpenGLType(graphics::TextureFiltering filter) {
		switch (filter) {
		case graphics::TextureFiltering::NEAREST: return GL_NEAREST;
		case graphics::TextureFiltering::LINEAR:  return GL_LINEAR;
		}

		ASSERTM(false, "[OpenGL] Invalid Texture Filter");
		return GL_INVALID_ENUM;
	}

	GLenum OpenGLHelper::GetOpenGLType(graphics::TextureWrapping wrap) {
		switch (wrap) {
		case graphics::TextureWrapping::REPEAT:	return GL_REPEAT;
		case graphics::TextureWrapping::CLAMP:	return GL_CLAMP_TO_EDGE;
		}

		ASSERTM(false, "[OpenGL] Invalid Texture Wrapping");
		return GL_INVALID_ENUM;
	}

	GLenum OpenGLHelper::GetTextureInternalFormat(graphics::TextureFormat format) {
		switch (format) {
		case graphics::TextureFormat::R:	return GL_R8;
		case graphics::TextureFormat::RG:	return GL_RG8;
		case graphics::TextureFormat::RGB:	return GL_RGB8;
		case graphics::TextureFormat::RGBA:	return GL_RGBA8;
		}

		ASSERTM(false, "[OpenGL] Invalid Texture Format");
		return GL_INVALID_ENUM;
	}

	unsigned int OpenGLHelper::GetNumBytes(graphics::TextureFormat format) {
		switch (format) {
		case graphics::TextureFormat::R:	return 1u;
		case graphics::TextureFormat::RG:	return 2u;
		case graphics::TextureFormat::RGB:	return 3u;
		case graphics::TextureFormat::RGBA:	return 4u;
		}

		ASSERTM(false, "[OpenGL] Invalid Texture Format");
		return GL_INVALID_ENUM;
	}

} } }

#endif //__OPENGL__