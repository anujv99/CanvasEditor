
/*
 *  Implementation of the graphics render state class interface
 */

#ifdef __OPENGL__

#ifndef __OPENGL_RENDER_STATE_H__
#define __OPENGL_RENDER_STATE_H__

#include "openglhelper.h"

#include <graphics/renderstate.h>

namespace app { namespace external { namespace opengl {

    class OpenGLRenderState : public graphics::RenderState {
    public:
        OpenGLRenderState();
        ~OpenGLRenderState();
    public:
        virtual void SetTopology(graphics::Topology topology) override;
        virtual void SetViewport(graphics::Viewport viewport) override;
        virtual void SetBlendFunction(graphics::BlendFunction func) override;

        inline virtual graphics::Topology GetTopology() const { return m_Prim; };
        inline virtual graphics::Viewport GetViewport() const { return m_Viewport; };
        inline virtual graphics::BlendFunction GetBlendFunction() const { return m_BlendFunc; };

        inline GLenum GetOpenGLTopology() const { return m_OpenGLPrim; }
    private:
        graphics::Topology m_Prim;
        graphics::Viewport m_Viewport;
        graphics::BlendFunction m_BlendFunc;

        GLenum m_OpenGLPrim;
    };

} } }

#endif //__OPENGL_RENDER_STATE_H__

#endif //__OPENGL__
