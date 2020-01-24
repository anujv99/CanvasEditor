
/*
 * Conatins all the mouse events
 */

#ifndef __MOUSE_EVENT_H__
#define __MOUSE_EVENT_H__

#include "event.h"

namespace app { namespace core { namespace events {

	class MouseMovedEvent : public Event {
	public:
		MouseMovedEvent(float x, float y) :
			m_MousePosX(x), m_MousePosY(y) {}

		inline float GetMousePosX() const { return m_MousePosX; }
		inline float GetMousePosY() const { return m_MousePosY; }

		virtual std::string ToString() const override {
			std::stringstream ss;
			ss << "MouseMovedEvent: " << m_MousePosX << ", " << m_MousePosY;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseMoved)
			EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
	private:
		float m_MousePosX, m_MousePosY;
	};

	class MouseScrolledEvent : public Event {
	public:
		MouseScrolledEvent(float xOffset, float yOffset) :
			m_ScrollOffsetX(xOffset), m_ScrollOffsetY(yOffset) {}

		inline float GetScrollOffsetX() const { return m_ScrollOffsetX; }
		inline float GetScrollOffsetY() const { return m_ScrollOffsetY; }

		virtual std::string ToString() const override {
			std::stringstream ss;
			ss << "MouseScrolledEvent: " << m_ScrollOffsetX << ", " << m_ScrollOffsetY;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseScrolled)
			EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
	private:
		float m_ScrollOffsetX, m_ScrollOffsetY;
	};

	class MouseButtonEvent : public Event {
	public:
		inline int GetMouseButton() const { return m_Button; }

		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput | EventCategoryMouseButton)

	protected:
		MouseButtonEvent(int button) :
			m_Button(button) {}

		int m_Button;
	};

	class MouseButtonPressedEvent : public MouseButtonEvent {
	public:
		MouseButtonPressedEvent(int button) :
			MouseButtonEvent(button) {}

		virtual std::string ToString() const override {
			std::stringstream ss;
			ss << "MouseButtonPressedEvent: " << m_Button;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseButtonPressed)
	};

	class MouseButtonReleasedEvent : public MouseButtonEvent {
	public:
		MouseButtonReleasedEvent(int button) :
			MouseButtonEvent(button) {}

		virtual std::string ToString() const override {
			std::stringstream ss;
			ss << "MouseButtonReleasedEvent: " << m_Button;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseButtonReleased)
	};

} } }

#endif //__MOUSE_EVENT_H__
