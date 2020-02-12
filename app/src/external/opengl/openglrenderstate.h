
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
        virtual void SetLineThickness(float thickness) override;

        inline virtual graphics::Topology GetTopology() const override { return m_Prim; };
        inline virtual graphics::Viewport GetViewport() const override { return m_Viewport; };
        inline virtual graphics::BlendFunction GetBlendFunction() const override { return m_BlendFunc; };
        inline virtual float GetLineThickness() const override { return m_LineThickness; };

        inline GLenum GetOpenGLTopology() const { return m_OpenGLPrim; }
    private:
        graphics::Topology m_Prim;
        graphics::Viewport m_Viewport;
        graphics::BlendFunction m_BlendFunc;
        float m_LineThickness;
        GLenum m_OpenGLPrim;
    };

} } }

#endif //__OPENGL_RENDER_STATE_H__

#endif //__OPENGL__
