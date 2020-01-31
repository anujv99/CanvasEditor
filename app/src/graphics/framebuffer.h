
/*
 *  Interface to graphics API specific framebuffer object.
 *  Actual implementation is in the folder external/opengl
 */

#ifndef __FRAMEBUFFER_H__
#define __FRAMEBUFFER_H__

#include <utils/stronghandle.h>
#include <utils/handledobject.h>
#include <math/math.h>

#include "texture.h"

namespace app { namespace graphics {

    class Framebuffer : public utils::HandledObject<Framebuffer> {
        friend class utils::StrongHandle<Framebuffer>;
    protected:
        Framebuffer() {}
        virtual ~Framebuffer() {}
    public:
        virtual void Bind() = 0;
        virtual void UnBind() = 0;
        virtual void Clear() = 0;
        virtual void Clear(Vec4 color) = 0;
        virtual Vec2i GetSize() const = 0;
        virtual utils::StrongHandle<Texture> GetTexture() = 0;
    public:
        static utils::StrongHandle<Framebuffer> Create(Vec2i size, TextureFormat format); // Use this to create framebuffer
    };

} }

#endif //__FRAMEBUFFER_H__
