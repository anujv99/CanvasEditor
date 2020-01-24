
#ifndef __WINDOW_EVENT_H__
#define __WINDOW_EVENT_H__

#include "event.h"

namespace app { namespace core { namespace events {

	class WindowResizeEvent : public Event {
	public:
		WindowResizeEvent(int width, int height) :
			m_WindowSizeX(width), m_WindowSizeY(height) {}
		inline int GetWindowSizeX() const { return m_WindowSizeX; };
		inline int GetWindowSizeY() const { return m_WindowSizeY; };

		virtual std::string ToString() const override {
			std::stringstream ss;
			ss << "WindowResizeEvent: " << m_WindowSizeX << ", " << m_WindowSizeY;
			return ss.str();
		}

		EVENT_CLASS_TYPE(WindowResize)
			EVENT_CLASS_CATEGORY(EventCategoryApplication)
	private:
		int m_WindowSizeX, m_WindowSizeY;
	};

	class WindowCloseEvent : public Event {
	public:
		WindowCloseEvent() {}

		virtual std::string ToString() const override {
			std::stringstream ss;
			ss << "WindowCloseEvent";
			return ss.str();
		}

		EVENT_CLASS_TYPE(WindowClose)
			EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class WindowMoveEvent : public Event {
	public:
		WindowMoveEvent(int xpos, int ypos) :
			m_WindowPosX(xpos), m_WindowPosY(ypos) {}

		inline int GetWindowPosX() const { return m_WindowPosX; }
		inline int GetWindowPosY() const { return m_WindowPosY; }

		virtual std::string ToString() const override {
			std::stringstream ss;
			ss << "WindowMovedEvent: " << m_WindowPosX << ", " << m_WindowPosY;
			return ss.str();
		}

		EVENT_CLASS_TYPE(WindowMoved)
			EVENT_CLASS_CATEGORY(EventCategoryApplication)
	private:
		int m_WindowPosX, m_WindowPosY;
	};

	class AppTickEvent : public Event {
	public:
		AppTickEvent() {}

		EVENT_CLASS_TYPE(AppTick)
			EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class AppUpdateEvent : public Event {
	public:
		AppUpdateEvent() {}

		EVENT_CLASS_TYPE(AppUpdate)
			EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class AppRenderEvent : public Event {
	public:
		AppRenderEvent() {}

		EVENT_CLASS_TYPE(AppRender)
			EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

} } }

#endif //__WINDOW_EVENT_H__
