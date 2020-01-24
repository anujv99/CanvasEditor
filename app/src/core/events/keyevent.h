
/*
 * Conatins all the keyboard events
 */

#ifndef __KEY_EVENT_H__
#define __KEY_EVENT_H__

#include "event.h"

namespace app { namespace core { namespace events {

	class KeyEvent : public Event {
	public:
		inline int GetKeyCode() const { return m_KeyCode; }

		EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)
	protected:
		KeyEvent(int keyCode) :
			m_KeyCode(keyCode) {}

		int m_KeyCode;
	};

	class KeyPressedEvent : public KeyEvent {
	public:
		KeyPressedEvent(int keycode, bool repeatCount) :
			KeyEvent(keycode), m_Repeat(repeatCount) {}

		inline bool IsRepeating() const { return m_Repeat; }

		virtual std::string ToString() const override {
			std::stringstream ss;
			ss << "KeyPressedEvent: " << m_KeyCode << " (" << m_Repeat << " repeats)";
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyPressed)
	private:
		bool m_Repeat;
	};

	class KeyReleasedEvent : public KeyEvent {
	public:
		KeyReleasedEvent(int keycode) :
			KeyEvent(keycode) {}

		virtual std::string ToString() const override {
			std::stringstream ss;
			ss << "KeyReleasedEvent: " << m_KeyCode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyReleased)
	};

	class CharacterEvent : public Event {
	public:
		CharacterEvent(char pressedChar) :
			m_PressedChar(pressedChar) {}

		inline char GetPressedChar() const { return m_PressedChar; }

		virtual std::string ToString() const override {
			std::stringstream ss;
			ss << "CharacterEvent: " << m_PressedChar;
			return ss.str();
		}

		EVENT_CLASS_TYPE(CharacterInput)
			EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)
	private:
		char m_PressedChar;
	};

} } }

#endif //__KEY_EVENT_H__
