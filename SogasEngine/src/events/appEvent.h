#pragma once

#include "event.h"

namespace Sogas 
{
	class windowResizeEvent : public Event
	{
	public:
		windowResizeEvent(u32 width, u32 height)
			: m_width(width), m_height(height)
		{
		}

		u32 getWidth() const { return m_width; }
		u32 getHeight() const { return m_height; }

		std::string toString() const override
		{
			std::stringstream ss;
			ss << "windowResizeEvent: " << m_width << ", " << m_height;
			return ss.str();
		}

		EVENT_CLASS_TYPE(WindowResize)
			EVENT_CLASS_CATEGORY(EventCategoryApplication)

	private:
		u32 m_width, m_height;
	};

	class windowCloseEvent : public Event
	{
	public:
		windowCloseEvent() = default;

		EVENT_CLASS_TYPE(WindowClose)
			EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};
}