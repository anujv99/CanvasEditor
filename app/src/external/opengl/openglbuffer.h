
/*
 *  Implementation of the graphics buffer class interface
 */

#ifdef __OPENGL__

#ifndef __OPENGL_BUFFER_H__
#define __OPENGL_BUFFER_H__

#include "openglhelper.h"
#include <graphics/buffer.h>
#include <graphics/bufferlayout.h>

namespace app { namespace external { namespace opengl {

    //------------- VERTEX BUFFER -------------

    class OpenGLVertexBuffer : public app::graphics::VertexBuffer {
    public:
        OpenGLVertexBuffer(const void * data, size_t size, size_t stride, graphics::BufferUsage usage);
        ~OpenGLVertexBuffer();
    public:
        virtual void Bind() const override;
        virtual void UnBind() const override;
        virtual void SubData(const void * data, size_t size, size_t offset) override;
        virtual void * Map() override;
        virtual void UnMap() override;
        virtual void SetBufferLayout(utils::StrongHandle<graphics::BufferLayout> & layout) override;

        inline virtual size_t GetStride() const override { return m_Size; }
        inline virtual size_t GetSize() const override { return m_Size; }
    private:
        GLuint m_ID;
        size_t m_Size;
        size_t m_Stride;
        utils::StrongHandle<graphics::BufferLayout> m_Layout;
        graphics::BufferUsage m_Usage;
        bool m_IsMapped;
    };

    // ------------- UNIFORM BUFFER -------------

    class OpenGLUniformBuffer : public app::graphics::UniformBuffer {
    public:
        OpenGLUniformBuffer(const void * data, size_t size, graphics::BufferUsage usage);
        ~OpenGLUniformBuffer();
    public:
        virtual void Bind(unsigned int block) const override;
        virtual void UnBind() const override;
        virtual void SubData(const void * data, size_t size, size_t offset) override;
    private:
        GLuint m_ID;
        size_t m_Size;
        mutable GLuint m_Block;
    };

} } }

#endif //__OPENGL_BUFFER_H__

#endif //__OPENGL__