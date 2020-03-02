
/*
 *  Implementation of the graphics shader class interface
 */

#ifdef __OPENGL__

#ifndef __OPENGL_SHADER_H__
#define __OPENGL_SHADER_H__

#include "openglhelper.h"

#include <graphics/shader.h>

namespace app { namespace external { namespace opengl {

    // ------------- VERTEX SHADER -------------

    class OpenGLVertexShader : public graphics::VertexShader {
    public:
        OpenGLVertexShader(const char * source);
        ~OpenGLVertexShader();

        inline GLuint GetID() const { return m_ID; }
    private:
        GLuint m_ID;
    };

    // ------------- FRAGMENT SHADER -------------

    class OpenGLFragmentShader : public graphics::FragmentShader {
    public:
        OpenGLFragmentShader(const char * source);
        ~OpenGLFragmentShader();

        inline GLuint GetID() const { return m_ID; }
    private:
        GLuint m_ID;
    };

    // ------------- SHADER PROGRAM -------------

    class OpenGLShaderProgram : public graphics::ShaderProgram {
    public:
        OpenGLShaderProgram(utils::StrongHandle<graphics::VertexShader> vShader, utils::StrongHandle<graphics::FragmentShader> fShader);
        ~OpenGLShaderProgram();

        inline GLuint GetID() const { return m_ID; }

        inline virtual void Bind() override { glUseProgram(m_ID); }
        inline virtual void UnBind() override { glUseProgram(0); }
        virtual int GetUniformBlockLocation(const char * uniformName) override;
        virtual int GetUniformLocation(const char * uniformName) override;

        inline virtual void SetUniformVec2(int uniformLocation, Vec2 val) override { glUniform2f(uniformLocation, val.x, val.y); }
        inline virtual void SetUniformFloat(int uniformLocation, float val) override { glUniform1f(uniformLocation, val); }
    private:
        GLuint m_ID;
        utils::StrongHandle<graphics::VertexShader> m_VS;
        utils::StrongHandle<graphics::FragmentShader> m_FS;
    };

} } }

#endif //__OPENGL_SHADER_H__

#endif //__OPENGL__
