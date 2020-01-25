#include "openglbuffer.h"

#ifdef __OPENGL__

namespace app {

	//------------- VERTEX BUFFER -------------

	namespace graphics {
		utils::StrongHandle<VertexBuffer> VertexBuffer::Create(const void * data, size_t size, size_t stride, BufferUsage usage) {
			return dynamic_cast<VertexBuffer *>(new external::opengl::OpenGLVertexBuffer(data, size, stride, usage));
		}
	}

	namespace external { namespace opengl {

		OpenGLVertexBuffer::OpenGLVertexBuffer(const void * data, size_t size, size_t stride, graphics::BufferUsage usage) :
			m_ID(0u), m_IsMapped(false), m_Size(size), m_Stride(stride), m_Usage(usage), m_Layout(nullptr) {
			ASSERTM(size > 0 && stride > 0, "[OpenGL] Invalid size or stride for a vertex buffer");
			ASSERTM(usage != graphics::BufferUsage::STATIC || data, "[OpenGL] Buffer with static usage must be initialized with some data");
			glCreateBuffers(1, &m_ID);
			glNamedBufferData(m_ID, size, data, OpenGLHelper::GetOpenGLType(usage));
			ASSERTM(m_ID > 0, "[OpenGL] Failed to create vertex buffer");
		}

		OpenGLVertexBuffer::~OpenGLVertexBuffer() {
			glDeleteBuffers(1, &m_ID);
		}

		void OpenGLVertexBuffer::Bind() const {
			glBindBuffer(GL_ARRAY_BUFFER, m_ID);
		}

		void OpenGLVertexBuffer::UnBind() const {
			glBindBuffer(GL_ARRAY_BUFFER, 0u);
		}

		void OpenGLVertexBuffer::SubData(const void * data, size_t size, size_t offset) {
			ASSERTM(data, "[OpenGL] Invalid data");
			ASSERTM(size + offset <= m_Size, "[OpenGL] Invalid Size");
			ASSERTM(m_Usage != graphics::BufferUsage::STATIC, "[OpenGL] Cannot use SubData on buffer with Static usage");
			glNamedBufferSubData(m_ID, (GLintptr)offset, (GLsizeiptr)size, data);
		}

		void * OpenGLVertexBuffer::Map() {
			ASSERTM(m_Usage != graphics::BufferUsage::STATIC, "[OpenGL] Cannot use Map on buffer with Static usage");
			ASSERTM(m_IsMapped == false, "[OpenGL] Buffer already mapped. Use UnMap after calling Map");

			void * mb = glMapNamedBuffer(m_ID, GL_WRITE_ONLY);
			ASSERTM(mb, "[OpenGL] Failed to map buffer");
			m_IsMapped = true;

			return mb;
		}

		void OpenGLVertexBuffer::UnMap() {
			ASSERTM(m_IsMapped == true, "[OpenGL] Buffer not mapped. Use Map before calling UnMap");

			ASSERTM(glUnmapNamedBuffer(m_ID) == GL_TRUE, "[OpenGL] Failed to UnMap buffer");
			m_IsMapped = false;
		}

		void OpenGLVertexBuffer::SetBufferLayout(utils::StrongHandle<graphics::BufferLayout> & layout) {
			if (layout == nullptr) {
				LOG_ERROR("Inavlid buffer layout");
				return;
			}
			m_Layout = layout;
		}

	} }

	//------------- UBIFORM BUFFER -------------

	namespace graphics {
		utils::StrongHandle<UniformBuffer> UniformBuffer::Create(const void * data, size_t size, BufferUsage usage) {
			return dynamic_cast<UniformBuffer *>(new external::opengl::OpenGLUniformBuffer(data, size, usage));
		}
	}

	namespace external { namespace opengl {

		OpenGLUniformBuffer::OpenGLUniformBuffer(const void * data, size_t size, graphics::BufferUsage usage) : 
			m_ID(0u), m_Size(0u), m_Block(0u) {
			ASSERTM(size > 0, "[OpenGL] Invalid buffer size");
			ASSERTM(usage != graphics::BufferUsage::STATIC || data, "[OpenGL] Buffer with static usage must be initialized with some data");

			if (size % 16 != 0) {
				size += 16 - (size % 16);
			}

			m_Size = size;
			glCreateBuffers(1, &m_ID);
			ASSERTM(m_ID > 0, "[OpenGL] Failed to create uniform buffer");
			glNamedBufferData(m_ID, size, data, OpenGLHelper::GetOpenGLType(usage));
		}

		OpenGLUniformBuffer::~OpenGLUniformBuffer() {
			glDeleteBuffers(1, &m_ID);
		}

		void OpenGLUniformBuffer::Bind(unsigned int block) const {
			glBindBufferBase(GL_UNIFORM_BUFFER, (GLuint)block, m_ID);
			m_Block = block;
		}

		void OpenGLUniformBuffer::UnBind() const {
			glBindBufferBase(GL_UNIFORM_BUFFER, (GLuint)m_Block, 0);
		}

		void OpenGLUniformBuffer::SubData(const void * data, size_t size, size_t offset) {
			ASSERTM(data, "[OpenGL] Invalid data");
			ASSERTM(size + offset <= m_Size, "[OpenGL] Invalid Size");
			glNamedBufferSubData(m_ID, (GLintptr)offset, GLsizeiptr(size), data);
		}

	} }

}

#endif
