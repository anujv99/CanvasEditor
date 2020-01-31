
/* 
 *  Interface to the window class
 *  The actual implimentation is in the external/glfw folder
 *  This software uses GLFW for window creation but can be changed easily
 *  Window is singleton because there can be only one window
 */

#ifndef __WINDOW_H__
#define __WINDOW_H__

#include <functional>

#include <utils/singleton.h>
#include <core/events/event.h>

namespace app {

	namespace core {

        class Window : public app::utils::Singleton<Window> {
            friend class app::utils::Singleton<Window>;
        protected:
            Window() {}
            virtual ~Window() {}
        public:
            virtual void Create(unsigned int width, unsigned int height, const char * title) = 0;
            virtual void Update() = 0;
            virtual uintptr_t GetRawWindow() = 0;
            virtual void SetEventFunction(std::function<void(events::Event &)> eventFunc) = 0;

            virtual unsigned int GetWidth() const = 0;
            virtual unsigned int GetHeight() const = 0;
        private:
            static Window * CreateWindow(); // This function should be implemented in order to create a window
        };

	}

    namespace utils {

        template<>
        inline void Singleton<app::core::Window>::CreateInst() {
            if (!s_Instance) s_Instance = app::core::Window::CreateWindow();
        }

    }

}

#endif
