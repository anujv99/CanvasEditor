#include "renderer.h"

namespace app { namespace renderer {

	#include "shaders.inl"	//Contains shaders

	using namespace graphics;

	Renderer::Renderer() {
		CreateRendererObjects();
		CreateFramebufferObjects();

		m_MVPBuffer = graphics::UniformBuffer::Create(Mat4().GetFloatPtr(), sizeof(Mat4), graphics::BufferUsage::DYNAMIC);
		m_MVPBuffer->Bind(m_RendererShader->GetUniformBlockLocation("MVP"));
	}

	Renderer::~Renderer() {}

	void Renderer::PassFramebuffer(utils::StrongHandle<graphics::Framebuffer> from, utils::StrongHandle<graphics::Framebuffer> to) {
		ASSERTM(from != nullptr, "Cannot sample from default framebuffer");
		if (to != nullptr) {
			to->Bind();
		} else {
			from->UnBind();
		}
		

		m_FramebufferRendererVertexArray->Bind();
		m_FramebufferRendererShader->Bind();

		from->GetTexture()->Bind(0);

		RenderState::Ref().SetTopology(Topology::TRIANGLE);

		m_FramebufferRendererVertexArray->Draw(6);

		if (to != nullptr) {
			to->UnBind();
		}
	}

	void Renderer::CreateRendererObjects() {
		m_RendererBuffer = VertexBuffer::Create(nullptr, MAX_VERTICES * sizeof(RendererVertex), BufferUsage::DYNAMIC);

		utils::StrongHandle<BufferLayout> layout = BufferLayout::Create();
		layout->BeginEntries();
		layout->AddEntry(DataType::Float2, offsetof(RendererVertex, Position), "POSITION", false);
		layout->AddEntry(DataType::Float2, offsetof(RendererVertex, TexCoord), "TEX_COORD", false);
		layout->AddEntry(DataType::Int, offsetof(RendererVertex, TexIndex), "TEX_INDEX", false);
		layout->AddEntry(DataType::UByte4, offsetof(RendererVertex, col), "COLOR", true);
		layout->EndEntries();

		m_RendererBuffer->SetBufferLayout(layout);

		utils::StrongHandle<VertexShader> vShader;
		utils::StrongHandle<FragmentShader> fShader;

		vShader = VertexShader::Create(GL_V_RENDERER_SHADER);
		fShader = FragmentShader::Create(GL_F_RENDERER_SHADER);

		m_RendererShader = ShaderProgram::Create(vShader, fShader);

		m_RendererVertexArray = VertexArray::Create();
		m_RendererVertexArray->AddVertexBuffer(m_RendererBuffer);
	}

	void Renderer::CreateFramebufferObjects() {

		float vertices[] = {
			-1.0f,  1.0f, 0.0f, 1.0f,
			 1.0f,  1.0f, 1.0f, 1.0f,
			 1.0f, -1.0f, 1.0f, 0.0f,

			-1.0f,  1.0f, 0.0f, 1.0f,
			 1.0f, -1.0f, 1.0f, 0.0f,
			-1.0f, -1.0f, 0.0f, 0.0f,
		};

		auto fbobuffer = VertexBuffer::Create(vertices, sizeof(vertices), BufferUsage::STATIC);

		utils::StrongHandle<BufferLayout>  layout = BufferLayout::Create();
		layout->BeginEntries();
		layout->AddEntry(DataType::Float2, 0, "POSITION", false);
		layout->AddEntry(DataType::Float2, 2 * sizeof(float), "TEX_COORD", false);
		layout->EndEntries();

		fbobuffer->SetBufferLayout(layout);

		auto vShader = VertexShader::Create(GL_V_FRAMEBUFFER_SHADER);
		auto fShader = FragmentShader::Create(GL_F_FRAMEBUFFER_SHADER);

		m_FramebufferRendererShader = ShaderProgram::Create(vShader, fShader);

		m_FramebufferRendererVertexArray = VertexArray::Create();
		m_FramebufferRendererVertexArray->AddVertexBuffer(fbobuffer);
	}

	int Renderer::SubmitTexture(utils::StrongHandle<Texture> texture) {
		if (texture == nullptr) return -1;
		if (m_Textures.size() > MAX_TEXTURES) {
			EndScene();
		}
		for (size_t i = 0; i < m_Textures.size(); i++) {
			if (m_Textures[i] == texture) {
				return (int)i;
			}
		}
		m_Textures.push_back(texture);
		return ((int)m_Textures.size() - 1);
	}


	void Renderer::BeginScene() {
		if (math::MVPStack::Ref().IsChanged()) {
			m_MVPBuffer->SubData(static_cast<const void *>(math::MVPStack::Ref().GetMVP().GetFloatPtr()), sizeof(Mat4), 0);
		}
		m_Vertices = (RendererVertex *)m_RendererBuffer->Map();
	}

	void Renderer::EndScene() {
		m_RendererBuffer->UnMap();
		if (m_VertexIndex <= 0) return;

		RenderState::Ref().SetTopology(Topology::TRIANGLE);

		for (size_t i = 0; i < m_Textures.size(); i++) {
			m_Textures[i]->Bind((unsigned int)i);
		}

		m_RendererVertexArray->Bind();
		m_RendererShader->Bind();
		m_RendererVertexArray->Draw((unsigned int)m_VertexIndex);
		
		m_Textures.resize(0);

		m_VertexIndex = 0;
	}

	void Renderer::DrawSprite(Vec2 pos, Vec2 size, Vec4 color) {
		DrawSprite(pos, size, color, Vec2(0.0f), Vec2(0.0f), nullptr);
	}

	void Renderer::DrawSprite(Vec2 pos, Vec2 size, Vec4 color, Vec2 texCoordTopLeft,
		Vec2 texCoordBottomRight, utils::StrongHandle<graphics::Texture> texture) {
		
		ASSERTM(m_Vertices, "[Renderer] Invalid state. Did you forget to call BeginScene before drawing ?");

		if (m_VertexIndex + 6 >= MAX_VERTICES) { EndScene(); BeginScene(); }

		int texIndex = SubmitTexture(texture);

		m_Vertices[m_VertexIndex++] = { pos + Vec2(-size.x, size.y) / 2.0f, texCoordTopLeft, texIndex,
			(unsigned char)(color.x * 255.0f),
			(unsigned char)(color.y * 255.0f),
			(unsigned char)(color.z * 255.0f),
			(unsigned char)(color.w * 255.0f)
		};

		m_Vertices[m_VertexIndex++] = { pos + Vec2(size.x, size.y) / 2.0f, Vec2(texCoordBottomRight.x, texCoordTopLeft.y), texIndex,
			(unsigned char)(color.x * 255.0f),
			(unsigned char)(color.y * 255.0f),
			(unsigned char)(color.z * 255.0f),
			(unsigned char)(color.w * 255.0f)
		};

		m_Vertices[m_VertexIndex++] = { pos + Vec2(size.x, -size.y) / 2.0f, texCoordBottomRight, texIndex,
			(unsigned char)(color.x * 255.0f),
			(unsigned char)(color.y * 255.0f),
			(unsigned char)(color.z * 255.0f),
			(unsigned char)(color.w * 255.0f)
		};

		m_Vertices[m_VertexIndex++] = { pos + Vec2(-size.x, size.y) / 2.0f, texCoordTopLeft, texIndex,
			(unsigned char)(color.x * 255.0f),
			(unsigned char)(color.y * 255.0f),
			(unsigned char)(color.z * 255.0f),
			(unsigned char)(color.w * 255.0f)
		};

		m_Vertices[m_VertexIndex++] = { pos + Vec2(size.x, -size.y) / 2.0f, texCoordBottomRight, texIndex,
			(unsigned char)(color.x * 255.0f),
			(unsigned char)(color.y * 255.0f),
			(unsigned char)(color.z * 255.0f),
			(unsigned char)(color.w * 255.0f)
		};

		m_Vertices[m_VertexIndex++] = { pos + Vec2(-size.x, -size.y) / 2.0f, Vec2(texCoordTopLeft.x, texCoordBottomRight.y), texIndex,
			(unsigned char)(color.x * 255.0f),
			(unsigned char)(color.y * 255.0f),
			(unsigned char)(color.z * 255.0f),
			(unsigned char)(color.w * 255.0f)
		};

	}

	void Renderer::DrawText(utils::StrongHandle<Font> font, const Label & label) {
		if (font == nullptr) {
			LOG_ERROR("Invalid font");
			return;
		}
		Vec2 cursorPos = label.Pos;
		const Vec2 size = font->GetSize(label.Text) * label.Scale;

		for (size_t i = 0; i < label.Text.size(); i++) {
			unsigned char c = label.Text[i];
			const Character & character = font->GetCharacter(c);
			Vec2 topLeft = cursorPos + (Vec2(character.Size.x, -character.Size.y) * label.Scale) / 2.0f;
			topLeft += character.Offset * label.Scale;
			switch (label.Align) {
			case Label::Alignment::CENTER:
				topLeft -= size / 2.0f; break;
			case Label::Alignment::LEFT:
				topLeft -= Vec2(size.x, size.y / 2.0f); break;
			case Label::Alignment::RIGHT:
				topLeft -= Vec2(0.0f, size.y / 2.0f); break;
			}

			DrawSprite(topLeft, character.Size * label.Scale, label.Color, character.TexCoordTopLeft,
				character.TexCoordBottomRight, font->GetTexture());
			cursorPos.x += character.XAdvance * label.Scale.x;
		}
	}

} }