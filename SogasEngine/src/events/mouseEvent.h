#pragma once

#include "defines.h"

#include "event.h"

namespace Sogas 
{
	using MouseCode = u16;

	enum : MouseCode
	{
		Button0 = 0,
		Button1 = 1,
		Button2 = 2,
		Button3 = 3,
		Button4 = 4,
		Button5 = 5,
		Button6 = 6,
		Button7 = 7,

		ButtonLast = Button7,
		ButtonLeft = Button0,
		ButtonRight = Button1,
		ButtonMiddle = Button2
	};

	class MouseMoveEvent : public Event
	{
	public:
		MouseMoveEvent(const f32 x, const f32 y)
			: m_posX(x), m_posY(y) {}

		f32 getX() const { return m_posX; }
		f32 getY() const { return m_posY; }

		std::string toString() const override
		{
			std::stringstream ss;
			ss << "MouseMoveEvent: " << m_posX << ", " << m_posY;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseMoved)
			EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryMouse)

	private:
		f32 m_posX, m_posY;
	};

	class MouseScrolledEvent : public Event
	{
	public:

		MouseScrolledEvent(const f32 offsetX, const f32 offsetY)
			: m_offsetX(offsetX), m_offsetY(offsetY) {}

		f32 getOffsetX() const { return m_offsetX; }
		f32 getOffsetY() const { return m_offsetY; }

		std::string toString() const override
		{
			std::stringstream ss;
			ss << "MouseScrolledEvent: " << m_offsetX << ", " << m_offsetY;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseScrolled)
			EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryMouse)

	private:
		f32 m_offsetX, m_offsetY;
	};

	class MouseButtonEvent : public Event
	{
	public:
		u16 getMouseButton() const { return m_button; }

		EVENT_CLASS_CATEGORY(EventCategoryMouseButton | EventCategoryInput)

	protected:
		MouseButtonEvent(const MouseCode button)
			: m_button(button) {}

		MouseCode m_button;
	};

	class MouseButtonPressedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonPressedEvent(const MouseCode button)
			: MouseButtonEvent(button) {}

		std::string toString() const override
		{
			std::stringstream ss;
			ss << "MouseButtonPressedEvent: " << m_button;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseButtonPressed)
	};

	class MouseButtonReleasedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonReleasedEvent(const MouseCode button)
			: MouseButtonEvent(button) {}

		std::string toString() const override
		{
			std::stringstream ss;
			ss << "MouseButtonReleasedEvent: " << m_button;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseButtonReleased)
	};
}