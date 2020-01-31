#include "openglvertexarray.h"

#include "openglrenderstate.h"

#ifdef __OPENGL__

namespace app {

	namespace graphics {
		utils::StrongHandle<VertexArray> VertexArray::Create() {
			return dynamic_cast<VertexArray *>(new app::external::opengl::OpenGLVertexArray());
		}
	}

	namespace external { namespace opengl {

		OpenGLVertexArray::OpenGLVertexArray() : m_ID(0u), m_VBOIndex(0u), m_VertexBuffers() {
			glCreateVertexArrays(1, &m_ID);
			ASSERTM(m_ID != 0, "[OpenGL] Failed to create vertex array");
		}

		OpenGLVertexArray::~OpenGLVertexArray() {
			glDeleteVertexArrays(1, &m_ID);
		}

		void OpenGLVertexArray::Bind() {
			glBindVertexArray(m_ID);
		}

		void OpenGLVertexArray::UnBind() {
			glBindVertexArray(0u);
		}

		void OpenGLVertexArray::AddVertexBuffer(const utils::StrongHandle<graphics::VertexBuffer> vertexBuffer) {
			Bind();
			vertexBuffer->Bind();

			const utils::StrongHandle<graphics::BufferLayout> & layout = vertexBuffer->GetBufferLayout();
			ASSERTM(layout != nullptr, "[OpenGL] Buffer does not have a valid buffer layout");
			for (const auto & entry : *layout) {
				glEnableVertexAttribArray(m_VBOIndex);
				glVertexAttribPointer(
					m_VBOIndex,
					entry.GetNumElements(),
					OpenGLHelper::GetOpenGLType(entry.Type),
					entry.Normalize ? GL_TRUE : GL_FALSE,
					vertexBuffer->GetStride(),
					(const void *)entry.OffsetBytes
				);
				m_VBOIndex++;
			}

			m_VertexBuffers.push_back(vertexBuffer);
		}

		void OpenGLVertexArray::Draw(unsigned int numElements) {
			glDrawArrays(((OpenGLRenderState *)OpenGLRenderState::Get())->GetOpenGLTopology(), 0, numElements);
		}

	} } 
}

#endif //__OPENGL__