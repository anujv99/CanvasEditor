
static constexpr char GL_V_SHADER[] = R"(
		#version 450 core

		layout (std140, binding = 0) uniform MVP {
			mat4 projectionView;
		};

		layout (location = 0) in vec2 iPos;
		layout (location = 1) in vec4 iColor;

		out vec4 passColor;

		void main() {
			passColor = iColor;
			gl_Position = projectionView * vec4(iPos.x, iPos.y, 0.0f, 1.0f);
		}
)";

static constexpr char GL_F_SHADER[] = R"(
		#version 450 core

		out vec4 FragColor;

		in vec4 passColor;

		void main() {
			FragColor = passColor;
		}
)";

