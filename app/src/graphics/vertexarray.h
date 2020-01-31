
/*
 *  Interface to graphics API specific vertex array
 *  Actual implementation is in the folder external/opengl
 *  Vertex array feeds data present in the vertex buffer to the vertex shader described by the buffer layout
 */

#ifndef __VERTEX_ARRAY_H__
#define __VERTEX_ARRAY_H__

#include <utils/handledobject.h>
#include <utils/stronghandle.h>

#include "buffer.h"

namespace app { namespace graphics {

    class VertexArray : utils::HandledObject<VertexArray> {
        friend class utils::StrongHandle<VertexArray>;
    protected:
        VertexArray() {}
        virtual ~VertexArray() {}
    public:
        virtual void Bind() = 0;
        virtual void UnBind() = 0;
        virtual void AddVertexBuffer(const utils::StrongHandle<VertexBuffer> vertexBuffer) = 0;
        virtual void Draw(unsigned int numElements) = 0; // Actual draw call. Try to reduce calls to this
    public:
        static utils::StrongHandle<VertexArray> Create();
    };

} }

#endif //__VERTEX_ARRAY_H__
