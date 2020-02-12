#include "immgfx.h"

namespace app { namespace renderer {
	
	using namespace utils;
	using namespace graphics;

	#include "immgfxshaders.inl"

	static constexpr const size_t MAX_VERTICES = 1024;

	ImmGFX::ImmGFX() : Batcher(MAX_VERTICES), m_CurrentBatch(nullptr) {
		m_Layout = BufferLayout::Create();
		m_Layout->BeginEntries();
		m_Layout->AddEntry(DataType::Float2, offsetof(renderer::Vertex, Position), "POSITION", false);
		m_Layout->AddEntry(DataType::UByte4, offsetof(renderer::Vertex, Color), "COLOR", true);
		m_Layout->EndEntries();

		StrongHandle<FragmentShader> fShader;
		StrongHandle<VertexShader> vShader;

		vShader = VertexShader::Create(GL_V_SHADER);
		fShader = FragmentShader::Create(GL_F_SHADER);
		
		m_Shader = ShaderProgram::Create(vShader, fShader);

	}

	ImmGFX::~ImmGFX() {}

	void ImmGFX::Render() {
		m_Shader->Bind();
		for (auto & batch : m_Batches) {
			if (batch.second->VBOIndex <= 0) continue;

			RenderState::Ref().SetTopology(batch.first);

			batch.second->VBO->UnMap();

			batch.second->VAO->Bind();
			batch.second->VAO->Draw(batch.second->VBOIndex);

			batch.second->Data = reinterpret_cast<renderer::Vertex *>(batch.second->VBO->Map());
			batch.second->VBOIndex = 0;
		}
	}

	void ImmGFX::PolygonBegin(Topology topology) {
		ASSERTM(m_CurrentBatch == nullptr, "Forgot to call PolygonEnd");
		m_CurrentBatch = GetBatch(topology);
	}

	void ImmGFX::PolygonEnd() {
		ASSERTM(m_CurrentBatch != nullptr, "Forgot to call PolygonBegin");
		m_CurrentBatch = nullptr;
	}

	void ImmGFX::Vertex(const Vec2 & pos) {
		m_CurrentBatch->Data[m_CurrentBatch->VBOIndex++] = { pos, m_CurrentColor };
	}

	StrongHandle<ImmGFX::Batch> ImmGFX::GetBatch(Topology topology) {
		auto it = m_Batches.find(topology);

		StrongHandle<Batch> batch;

		if (it == m_Batches.end()) {
			batch = new Batch(MAX_VERTICES * sizeof(renderer::Vertex), sizeof(renderer::Vertex), m_Layout);
			m_Batches[topology] = batch;
		} else {
			batch = it->second;
		}

		return batch;
	}

	ImmGFX::Batch::Batch(size_t bufferSize, size_t stride, StrongHandle<BufferLayout> layout) :
		VBOIndex(0), Data(nullptr) {
		ASSERT(bufferSize > 0 && stride > 0);
		VBO = VertexBuffer::Create(nullptr, bufferSize, stride, BufferUsage::DYNAMIC);
		VBO->SetBufferLayout(layout);

		VAO = VertexArray::Create();
		VAO->AddVertexBuffer(VBO);

		Data = reinterpret_cast<renderer::Vertex *>(VBO->Map());
	}

} }