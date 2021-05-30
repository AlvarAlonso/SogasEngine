#pragma once

#include "event.h"
#include "defines.h"
#include <sstream>

class KeyEvent : public Event
{
public:

	inline u32 getKeyCode() const { return m_keyCode; }

	EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)

protected:
	KeyEvent(u32 keycode)
		: m_keyCode(keycode) {}

	u32 m_keyCode;
};

class KeyPressedEvent : public KeyEvent
{
public:
	KeyPressedEvent(u32 keycode, u32 repeatCount)
		: KeyEvent(keycode), m_repeatCount(repeatCount) {}

	inline u32 getRepeatCount() const { return m_repeatCount; }

	std::string toString() const override
	{
		std::stringstream ss;
		ss << "KeyPressedEvent: " << m_keyCode << " (" << m_repeatCount << " repeats).";
		return ss.str();
	}

	EVENT_CLASS_TYPE(KeyPressed)

private:
	u32 m_repeatCount;
};

class KeyReleasedEvent : public KeyEvent
{
public:
	KeyReleasedEvent(u32 keycode)
		: KeyEvent(keycode) {}

	std::string toString() const override
	{
		std::stringstream ss;
		ss << "KeyReleasedEvent " << m_keyCode;
		return ss.str();
	}

	EVENT_CLASS_TYPE(KeyReleased);
};