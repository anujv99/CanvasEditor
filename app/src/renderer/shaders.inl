
static constexpr char GL_V_RENDERER_SHADER[] = R"(
		#version 450 core

		layout (std140, binding = 0) uniform MVP {
			mat4 projectionView;
		};

		layout (location = 0) in vec2 iPos;
		layout (location = 1) in vec2 iTexCoord;
		layout (location = 2) in int iTexIndex;
		layout (location = 3) in vec4 iColor;

		out vec2 passTexCoord;
		out vec4 passColor;
		flat out int passTexIndex;

		void main() {
			passTexCoord = iTexCoord;
			passTexIndex = iTexIndex;
			passColor = iColor;
			gl_Position = projectionView * vec4(iPos.x, iPos.y, 0.0f, 1.0f);
		}
)";

static constexpr char GL_F_RENDERER_SHADER[] = R"(
		#version 450 core

		layout (binding = 0) uniform sampler2D tex0;
		layout (binding = 1) uniform sampler2D tex1;
		layout (binding = 2) uniform sampler2D tex2;
		layout (binding = 3) uniform sampler2D tex3;
		layout (binding = 4) uniform sampler2D tex4;
		layout (binding = 5) uniform sampler2D tex5;

		out vec4 FragColor;

		in vec2 passTexCoord;
		in vec4 passColor;
		flat in int passTexIndex;

		void main() {
			vec4 outCol;

			if (passTexIndex < 0)	    outCol = vec4(1.0f);					
			else if (passTexIndex == 0) outCol = texture(tex0, passTexCoord);
			else if (passTexIndex == 1) outCol = texture(tex1, passTexCoord);
			else if (passTexIndex == 2) outCol = texture(tex2, passTexCoord);
			else if (passTexIndex == 3) outCol = texture(tex3, passTexCoord);
			else if (passTexIndex == 4) outCol = texture(tex4, passTexCoord);
			else if (passTexIndex == 5) outCol = texture(tex5, passTexCoord);

			FragColor = outCol * passColor;
		}
)";

static constexpr char GL_V_FRAMEBUFFER_SHADER[] = R"(
	#version 450 core

	layout (location = 0) in vec2 iPos;
	layout (location = 1) in vec2 iTexCoord;

	out vec2 passTexCoord;

	void main() {
		passTexCoord = iTexCoord;
		gl_Position = vec4(iPos.xy, 0.0f, 1.0f);
	}

)";

static constexpr char GL_F_FRAMEBUFFER_SHADER[] = R"(
	#version 450 core

	layout (binding = 0) uniform sampler2D tex;

	out vec4 FragColor;

	in vec2 passTexCoord;

	void main() {
		FragColor = texture(tex, passTexCoord);
	}

)";
