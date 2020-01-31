
/*
 *  Implementation of the graphics vertex array class interface
 */

#ifdef __OPENGL__

#ifndef __OPENGL_VERTEX_ARRAY_H__
#define __OPENGL_VERTEX_ARRAY_H__

#include <vector>

#include <graphics/vertexarray.h>
#include <graphics/buffer.h>
#include <utils/stronghandle.h>

#include "openglhelper.h"

namespace app { namespace external { namespace opengl {

	class OpenGLVertexArray : public graphics::VertexArray {
	public:
		OpenGLVertexArray();
		~OpenGLVertexArray();
	public:
		virtual void Bind() override;
		virtual void UnBind() override;
		virtual void AddVertexBuffer(const utils::StrongHandle<graphics::VertexBuffer> vertexBuffer) override;
		virtual void Draw(unsigned int numElements) override;
	private:
		GLuint m_ID;
		unsigned int m_VBOIndex;
		std::vector<utils::StrongHandle<graphics::VertexBuffer>> m_VertexBuffers;
	};

} } }

#endif //__OPENGL_VERTEX_ARRAY_H__

#endif //__OPENGL__
