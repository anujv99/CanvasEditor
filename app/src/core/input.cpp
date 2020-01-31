#include "input.h"

#include <cstring>

namespace app { namespace core {

	bool Input::m_Keys[MAX_KEYS]							= {};
	bool Input::m_PrevKeys[MAX_KEYS]						= {};

	bool Input::m_MouseButtons[MAX_MOUSE_BUTTONS]			= {};
	bool Input::m_PrevMouseButtons[MAX_MOUSE_BUTTONS]		= {};

	Vec2 Input::m_MousePos									= Vec2(0.0f);
	Vec2 Input::m_PrevMousePos								= Vec2(0.0f);
	Vec2 Input::m_MouseScrollDelta							= Vec2(0.0f);

	std::vector<char> Input::m_CharacterPressedBuffer;

	bool Input::IsKeyDown(unsigned short keyCode) {
		return m_Keys[keyCode];
	}

	bool Input::IsKeyPressed(unsigned short keyCode) {
		return m_Keys[keyCode] && !m_PrevKeys[keyCode];
	}

	bool Input::IsKeyUp(unsigned short keyCode) {
		return !m_Keys[keyCode];
	}

	bool Input::IsKeyReleased(unsigned short keyCode) {
		return !m_Keys[keyCode] && m_PrevKeys[keyCode];
	}

	bool Input::IsMouseButtonDown(unsigned short keyCode) {
		return m_MouseButtons[keyCode];
	}

	bool Input::IsMouseButtonPressed(unsigned short keyCode) {
		return m_MouseButtons[keyCode] && !m_PrevMouseButtons[keyCode];
	}

	bool Input::IsMouseButtonUp(unsigned short keyCode) {
		return !m_MouseButtons[keyCode];
	}

	bool Input::IsMouseButtonReleased(unsigned short keyCode) {
		return !m_MouseButtons[keyCode] && m_PrevMouseButtons[keyCode];
	}

	bool Input::IsCharacterPressed() {
		return !m_CharacterPressedBuffer.empty();
	}

	void Input::OnEvent(events::Event & e) {
		events::EventDispatcher dispatcher(e);
		dispatcher.Dispatch<events::KeyPressedEvent>(Input::KeyPressed);
		dispatcher.Dispatch<events::KeyReleasedEvent>(Input::KeyReleased);
		dispatcher.Dispatch<events::MouseMovedEvent>(Input::MouseMoved);
		dispatcher.Dispatch<events::MouseScrolledEvent>(Input::MouseScrolled);
		dispatcher.Dispatch<events::MouseButtonPressedEvent>(Input::MousePressed);
		dispatcher.Dispatch<events::MouseButtonReleasedEvent>(Input::MouseReleased);
		dispatcher.Dispatch<events::CharacterEvent>(Input::CharacterPressed);
	}

	bool Input::KeyPressed(events::KeyPressedEvent & e) {
		m_Keys[e.GetKeyCode()] = true;
		return false;
	}

	bool Input::KeyReleased(events::KeyReleasedEvent & e) {
		m_Keys[e.GetKeyCode()] = false;
		return false;
	}

	bool Input::MouseMoved(events::MouseMovedEvent & e) {
		m_MousePos = Vec2(e.GetMousePosX(), e.GetMousePosY());
		return false;
	}

	bool Input::MouseScrolled(events::MouseScrolledEvent & e) {
		m_MouseScrollDelta = Vec2(e.GetScrollOffsetX(), e.GetScrollOffsetY());
		return false;
	}

	bool Input::MousePressed(events::MouseButtonPressedEvent & e) {
		m_MouseButtons[e.GetMouseButton()] = true;
		return false;
	}

	bool Input::MouseReleased(events::MouseButtonReleasedEvent & e) {
		m_MouseButtons[e.GetMouseButton()] = false;
		return false;
	}

	bool Input::CharacterPressed(events::CharacterEvent & e) {
		m_CharacterPressedBuffer.push_back(e.GetPressedChar());
		return false;
	}

	void Input::Update() {
		m_PrevMousePos = m_MousePos;
		m_MouseScrollDelta = Vec2(0);
		std::memcpy(m_PrevKeys, m_Keys, sizeof(m_Keys));
		std::memcpy(m_PrevMouseButtons, m_MouseButtons, sizeof(m_MouseButtons));

		m_CharacterPressedBuffer.clear();
		m_CharacterPressedBuffer.reserve(MAX_CHARACTER_PER_FRAME);
	}

} }