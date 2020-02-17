
/*
 *  Implementation of the window class interface
 */

#ifndef __GLFW_WINDOW_H__
#define __GLFW_WINDOW_H__

#include <string>

#include <core/window.h>

//#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

namespace app { namespace external { namespace glfw {

    class GLFWWindow : public app::core::Window {
    public:
        GLFWWindow();
        ~GLFWWindow();
    public:
        virtual void Create(unsigned int width, unsigned int height, const char * title) override;
        virtual void Update() override;
        virtual uintptr_t GetRawWindow() override;
        virtual void SetEventFunction(std::function<void(app::core::events::Event &)> eventFunc) override;

        inline virtual unsigned int GetWidth() const override { return m_Data.Width; };
        inline virtual unsigned int GetHeight() const override { return m_Data.Height;  };
        inline virtual void GetRawMousePos(double * x, double * y) const override { glfwGetCursorPos(m_Window, x, y); };
    private:
        void SetCallbacks();
    private:
        bool m_IsCreated;
        ::GLFWwindow * m_Window;
        
        struct WindowData {
            unsigned int Width, Height;
            std::string Title;
            std::function<void(app::core::events::Event &)> EventFunc;
        };

        WindowData m_Data;

    };

} } }

#endif //__GLFW_WINDOW_H__
