
/*
 *  Interface to graphics API specific shader objects.
 *  Actual implementation is in the folder external/opengl
 */

#ifndef __SHADER_H__
#define __SHADER_H__

#include <string>

#include <utils/handledobject.h>

namespace app { namespace graphics {

    /*
     *  ------------- VERTEX SHADER -------------
     *  -----------------------------------------
     *  Vertex Shader is used to give positions to vertex
     *  ------""----- runs for every vertex
     */

    class VertexShader : public utils::HandledObject<VertexShader> {
        friend class utils::StrongHandle<VertexShader>;
    protected:
        VertexShader() {}
        virtual ~VertexShader() {}
    public:
        // This function should be implemented in order to create a vertex shader
        static utils::StrongHandle<VertexShader> Create(const char * source);
    };

    /*
     *  ------------- FRAGMENT SHADER -------------
     *  -----------------------------------------
     *  Fragment Shader is used to give color to pixels
     *  ------""----- runs for every pixel
     */

    class FragmentShader : public utils::HandledObject<FragmentShader> {
        friend class utils::StrongHandle<FragmentShader>;
    protected:
        FragmentShader() {}
        virtual ~FragmentShader() {}
    public:
        // This function should be implemented in order to create a fragment shader
        static utils::StrongHandle<FragmentShader> Create(const char * source);
    };

    /*
     *  ------------- SHADER PROGRAM -------------
     *  -----------------------------------------
     *  In OpenGL a draw call require atleast vertex and fragment shader to run
     *  A shader program consist of one vertex and one fragment shader and should be binded before a draw call
     */

    class ShaderProgram : public utils::HandledObject<ShaderProgram> {
        friend class utils::StrongHandle<ShaderProgram>;
    protected:
        ShaderProgram() {}
        virtual ~ShaderProgram() {}
    public:
        virtual void Bind() = 0;
        virtual void UnBind() = 0;
        virtual int GetUniformLocation(const char * uniformName) = 0;
    public:
        // This function should be implemented in order to create a shader program
        static utils::StrongHandle<ShaderProgram> Create(utils::StrongHandle<VertexShader> vShader, utils::StrongHandle<FragmentShader> fShader);
    };

} }

#endif //__SHADER_H__