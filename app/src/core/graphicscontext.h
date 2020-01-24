
/*
 *  Interface to graphics context
 *  The actual implimentation is in the external/opengl folder
 *  This software uses OpenGL 4.6 to draw but can be extended easily
 *  GraphicsContext is singleton because there is only one window
 */

#ifndef __GRAPHICS_CONTEXT_H__
#define __GRAPHICS_CONTEXT_H__

#include <utils/singleton.h>

namespace app {

    namespace core {

        class GraphicsContext : public app::utils::Singleton<GraphicsContext> {
            friend class app::utils::Singleton<GraphicsContext>;
        protected:
            GraphicsContext() {}
            virtual ~GraphicsContext() {}
        public:
            virtual void CreateContext(uintptr_t rawWindowPtr) = 0;
            virtual void BeginFrame() = 0;
            virtual void EndFrame() = 0;
        private:
            static GraphicsContext * CreateContext();
        };

    }

    namespace utils {

        template<>
        inline void Singleton<app::core::GraphicsContext>::CreateInst() {
            if (!s_Instance) s_Instance = app::core::GraphicsContext::CreateContext();
        }

    }

}

#endif //__GRAPHICS_CONTEXT_H__
