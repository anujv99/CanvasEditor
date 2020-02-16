#include "openglshader.h"

#ifdef __OPENGL__

namespace app {

	// Helper function to compile shaders and report any error

	GLuint CreateAndCompileShader(GLenum shaderType, const char * source) {
		ASSERTM(source, "[OpenGL] Invalid shader string");

		GLuint id = glCreateShader(shaderType);
		ASSERT(id);

		int success;
		char infoLog[512];

		glShaderSource(id, 1, &source, nullptr);
		glCompileShader(id);

		glGetShaderiv(id, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(id, sizeof(infoLog), nullptr, infoLog);
			switch (shaderType) {
			case GL_VERTEX_SHADER:
			{
				LOG_ERROR("Vertex Shader Compilation Failed:");
				break;
			}
			case GL_FRAGMENT_SHADER:
			{
				LOG_ERROR("Fragment Shader Compilation Failed:");
				break;
			}
			}
			LOG_ERROR("%s", infoLog);
			ASSERTM(false, "Fatal Error");
		}

		return id;
	}

	// ------------- VERTEX SHADER -------------

	namespace graphics {
		utils::StrongHandle<VertexShader> VertexShader::Create(const char * source) {
			return dynamic_cast<VertexShader *>(new app::external::opengl::OpenGLVertexShader(source));
		}
	}

	namespace external { namespace opengl {

		OpenGLVertexShader::OpenGLVertexShader(const char * source) : m_ID(0u) {
			m_ID = CreateAndCompileShader(GL_VERTEX_SHADER, source);
			LOG_INFO("[OpenGL] Vertex shader successfully created");
		}

		OpenGLVertexShader::~OpenGLVertexShader() {
			glDeleteShader(m_ID);
		}

	} }

	// ------------- FRAGMENT SHADER -------------

	namespace graphics {
		utils::StrongHandle<FragmentShader> FragmentShader::Create(const char * source) {
			return dynamic_cast<FragmentShader *>(new app::external::opengl::OpenGLFragmentShader(source));
		}
	}

	namespace external { namespace opengl {

		OpenGLFragmentShader::OpenGLFragmentShader(const char * source) : m_ID(0u) {
			m_ID = CreateAndCompileShader(GL_FRAGMENT_SHADER, source);
			LOG_INFO("[OpenGL] Fragment shader successfully created");
		}

		OpenGLFragmentShader::~OpenGLFragmentShader() {
			glDeleteShader(m_ID);
		}

	} }

	// ------------- SHADER PROGRAM -------------

	namespace graphics {
		utils::StrongHandle<ShaderProgram> ShaderProgram::Create(utils::StrongHandle<VertexShader> vShader, utils::StrongHandle<FragmentShader> fShader) {
			return dynamic_cast<ShaderProgram *>(new app::external::opengl::OpenGLShaderProgram(vShader, fShader));
		}
	}

	namespace external { namespace opengl {

		OpenGLShaderProgram::OpenGLShaderProgram(utils::StrongHandle<graphics::VertexShader> vShader, utils::StrongHandle<graphics::FragmentShader> fShader) : 
			m_ID(0u), m_VS(nullptr), m_FS(nullptr) {

			ASSERTM(vShader != nullptr, "[OpenGL] Invalid vertex shader");
			ASSERTM(fShader != nullptr, "[OpenGL] Invalid fragment shader");

			const OpenGLVertexShader * vs = dynamic_cast<const OpenGLVertexShader *>(vShader.Get());
			const OpenGLFragmentShader * fs = dynamic_cast<const OpenGLFragmentShader *>(fShader.Get());

			m_ID = glCreateProgram();
			glAttachShader(m_ID, vs->GetID());
			glAttachShader(m_ID, fs->GetID());
			glLinkProgram(m_ID);

			int success;
			char infoLog[512];

			glGetProgramiv(m_ID, GL_LINK_STATUS, &success);
			if (!success) {
				glGetProgramInfoLog(m_ID, 512, NULL, infoLog);
				LOG_ERROR("Shader Program Linking Failed:");
				LOG_ERROR("%s", infoLog);
				ASSERTM(false, "Fatal Error");
			}

			m_VS = vShader;
			m_FS = fShader;

			LOG_INFO("[OpenGL] Shader program successfully created");
		}

		OpenGLShaderProgram::~OpenGLShaderProgram() {
			glDeleteProgram(m_ID);
		}

		int OpenGLShaderProgram::GetUniformBlockLocation(const char * name) {
			int location = (int)glGetUniformBlockIndex(m_ID, name);
			if (location < 0) {
				LOG_ERROR("[OepnGL] Unable to find shader uniform block : %s", name);
			}
			return location;
		}

		int OpenGLShaderProgram::GetUniformLocation(const char * uniformName) {
			int location = (int)glGetUniformLocation(m_ID, uniformName);
			if (location < 0) {
				LOG_ERROR("[OepnGL] Unable to find shader uniform : %s", uniformName);
			}
			return location;
		}

	} }

}

#endif //__OPENGL__