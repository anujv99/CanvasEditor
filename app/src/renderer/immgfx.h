
#ifndef __IMMGFX_H__
#define __IMMGFX_H__

#include "batcher.h"

#include <utils/singleton.h>
#include <utils/handledobject.h>

#include <graphics/buffer.h>
#include <graphics/bufferlayout.h>
#include <graphics/shader.h>
#include <graphics/vertexarray.h>

namespace app { namespace renderer {

	class ImmGFX : public utils::Singleton<ImmGFX>, public Batcher {
		friend class utils::Singleton<ImmGFX>;
	private:
		ImmGFX();
		~ImmGFX();
	public:
		void Render();
	private:
		virtual void PolygonBegin(graphics::Topology topology) override;
		virtual void PolygonEnd() override;
		virtual void Vertex(const Vec2 & pos) override;
	private:
		utils::StrongHandle<graphics::ShaderProgram> m_Shader;
		utils::StrongHandle<graphics::BufferLayout> m_Layout;
		
		struct Batch : public utils:: HandledObject<Batch> {
			Batch(size_t bufferSize, utils::StrongHandle<graphics::BufferLayout> layout);

			utils::StrongHandle<graphics::VertexArray> VAO;
			utils::StrongHandle<graphics::VertexBuffer> VBO;
			size_t VBOIndex;
			renderer::Vertex * Data;
		};

		std::unordered_map<graphics::Topology, utils::StrongHandle<Batch>> m_Batches;
	private:
		utils::StrongHandle<Batch> GetBatch(graphics::Topology topology);
		utils::StrongHandle<Batch> m_CurrentBatch;
	};

} }

#endif //__IMMGFX_H__
