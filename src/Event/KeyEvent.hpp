#pragma once
#include <Input/Keyboard/KeyCodes.hpp>
#include "Event.hpp"
#include <string>
#include <sstream>

class KeyEvent : public Event
{
public:
	KeyCode GetKeyCode() const noexcept { return m_key_code; }

	EVENT_CLASS_CATEGORY(EventCategory::KEYBOARD | EventCategory::INPUT)
	
protected:
	explicit KeyEvent(const KeyCode keyCode)
		:
		m_key_code(keyCode)
	{}

	KeyCode m_key_code;
};

class KeyPressedEvent : public KeyEvent
{
public:
	KeyPressedEvent(const KeyCode keycode, const std::uint16_t repeat_count)
		:
		KeyEvent(keycode),
		m_repeat_count(repeat_count) {}

	std::uint16_t GetRepeatCount() const noexcept { return m_repeat_count; }

	std::string toString() const override
	{
		std::ostringstream ss;
		ss << "KeyPressedEvent: " << m_key_code << " (" << m_repeat_count << " repeats)";
		return ss.str();
	}

	EVENT_CLASS_TYPE(EventType::KEY_PRESSED)

private:
	std::uint16_t m_repeat_count;
};

class KeyReleasedEvent : public KeyEvent
{
public:
	explicit KeyReleasedEvent(const KeyCode keyCode)
		:
		KeyEvent(keyCode) 
	{}

	std::string toString() const override
	{
		std::ostringstream ss;
		ss << "KeyReleasedEvent: " << m_key_code;
		return ss.str();
	}

	EVENT_CLASS_TYPE(EventType::KEY_RELEASED)
};

class KeyTypedEvent : public KeyEvent
{
public:
	explicit KeyTypedEvent(const KeyCode keyCode)
		:
		KeyEvent(keyCode) {}

	std::string toString() const override
	{
		std::ostringstream ss;
		ss << "KeyTypedEvent: " << m_key_code;
		return ss.str();
	}

	EVENT_CLASS_TYPE(EventType::KEY_TYPED)
};
