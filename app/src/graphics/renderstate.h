
/*
 *  RenderState sets various parameters which can be changed during rendering, for eg. Topology, Blending, Stencil etc
 */

#ifndef __RENDER_STATE_H__
#define __RENDER_STATE_H__

#include <utils/singleton.h>

namespace app {

	namespace graphics {

		enum class Topology {
			POINT,
			LINE,
			LINE_ADJ,
			LINE_STRIP,
			LINE_STRIP_ADJ,
			TRIANGLE,
			TRIANGLE_ADJ,
			TRIANGLE_STRIP,
			TRIANGLE_STRIP_ADJ,
		};

		struct Viewport {
			float TopLeftX, TopLeftY;
			float Width, Height;
			float MinDepth, MaxDepth;
		};

		enum class BlendColorOption {
			ZERO,
			ONE,
			SRC_COLOR,
			INV_SRC_COLOR,
			DEST_COLOR,
			INV_DEST_COLOR,
			SRC_ALPHA,
			INV_SRC_ALPHA,
			DEST_ALPHA,
			INV_DEST_ALPHA
		};

		enum class BlendAlphaOption {
			ZERO,
			ONE,
			SRC_ALPHA,
			INV_SRC_ALPHA,
			DEST_ALPHA,
			INV_DEST_ALPHA
		};

		enum class BlendOperation {
			ADD,
			SUBTRACT,
			REV_SUBTRACT,
			MIN,
			MAX
		};

		struct BlendFunction {
			bool EnableBlending = true;

			BlendColorOption SrcColor, DestColor;
			BlendAlphaOption SrcAlpha, DestAlpha;

			BlendOperation ColorOperation, AlphaOperation;
		};

		class RenderState : public utils::Singleton<RenderState> {
			friend class utils::Singleton<RenderState>;
		protected:
			RenderState() {}
			virtual ~RenderState() {}
		public:
			virtual void SetTopology(Topology topology) = 0;
			virtual void SetViewport(Viewport viewport) = 0;
			virtual void SetBlendFunction(BlendFunction func) = 0;

			virtual Topology GetTopology() const = 0;
			virtual Viewport GetViewport() const = 0;
			virtual BlendFunction GetBlendFunction() const = 0;
		private:
			static RenderState * Create();
		};
	}

	namespace utils {

		template<>
		inline void Singleton<app::graphics::RenderState>::CreateInst() {
			if (!s_Instance) s_Instance = app::graphics::RenderState::Create();
		}

	}

}

#endif //__RENDER_STATE_H__
