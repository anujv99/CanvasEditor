
/*
 *  Interface to graphics API specific buffer objects.
 *  Actual implementation is in the folder external/opengl
 */

#ifndef __BUFFER_H__
#define __BUFFER_H__

#include <utils/handledobject.h>
#include <utils/stronghandle.h>

#include "bufferlayout.h"

namespace app { namespace graphics {

    /*
     *  Static usage is more faster but cannot be modified
     */
    enum class BufferUsage {
        STATIC,
        DYNAMIC,
    };

    /*
     *  ------------- VERTEX BUFFER -------------
     *  -----------------------------------------
     *  Vertex buffer is used to feed data per vertex to the shader pipeline 
     */

    class VertexBuffer : public app::utils::HandledObject<VertexBuffer> {
        friend class utils::StrongHandle<VertexBuffer>;
    protected:
        VertexBuffer() {}
        virtual ~VertexBuffer() {}
    public:
        virtual void Bind() const = 0;
        virtual void UnBind() const = 0;
        virtual void SubData(const void * data, size_t size, size_t offset) = 0; // Only buffer with dynamic usage can be modified using SubData
        virtual void * Map() = 0;
        virtual void UnMap() = 0;
        virtual void SetBufferLayout(utils::StrongHandle<BufferLayout> & layout) = 0;
        virtual size_t GetStride() const = 0;
        virtual size_t GetSize() const = 0;
    public:
        // This function should be implemented in order to create a vertex buffer
        static utils::StrongHandle<VertexBuffer> Create(const void * data, size_t size, size_t stride, BufferUsage usage);
    };

    /*
     *  ------------- UNIFORM BUFFER -------------
     *  -----------------------------------------
     *  Uniform buffer is used to feed data per draw call to the shader pipeline
     */

    class UniformBuffer : public utils::HandledObject<UniformBuffer> {
        friend class utils::StrongHandle<UniformBuffer>;
    protected:
        UniformBuffer() {}
        ~UniformBuffer() {}
    public:
        virtual void Bind(unsigned int block) const = 0;
        virtual void UnBind() const = 0;
        virtual void SubData(const void * data, size_t size, size_t offset) = 0; // Only buffer with dynamic usage can be modified using SubData
    public:
        // This function should be implemented in order to create a uniform buffer
        static utils::StrongHandle<UniformBuffer> Create(const void * data, size_t size, BufferUsage usage);
    };

} }

#endif //__BUFFER_H__
