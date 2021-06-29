#pragma once

#include "defines.h"
#include <string>
#include <sstream>

namespace Sogas 
{
	enum class e_eventType
	{
		None = 0,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
		AppTick, AppUpdate, AppRender,
		KeyPressed, KeyReleased, KeyTyped,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
	};

	enum class e_eventCategory
	{
		None = 0,
		EventCategoryApplication = BIT(0),
		EventCategoryInput = BIT(1),
		EventCategoryKeyboard = BIT(2),
		EventCategoryMouse = BIT(3),
		EventCategoryMouseButton = BIT(4)
	};

#define EVENT_CLASS_TYPE(type) static e_eventType getStaticType() { return e_eventType::##type; }\
								virtual e_eventType getEventType() const override { return getStaticType(); }\
								virtual const char* getName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) virtual u32 getCategoryFlags() const override { return category; }

	class Event
	{
	public:
		bool handled = false;

		virtual e_eventType getEventType() const = 0;
		virtual const char* getName() const = 0;
		virtual u32 getCategoryFlags() const = 0;
		virtual std::string toString() const { return getName(); }

		bool isInCategory(e_eventCategory category)
		{
			return getCategoryFlags() & static_cast<u32>(category);
		}
	};

	class EventDispatcher
	{
	public:
		EventDispatcher(Event& event)
			:m_event(event)
		{
		}

		template<typename T, typename F>
		bool dispatch(const F& func)
		{
			if (m_event.getEventType() == T::getStaticType())
			{
				m_event.handled |= func(static_cast<T&>(m_event));
				return true;
			}
			return false;
		}

	private:
		Event& m_event;
	};

	inline std::ostream& operator<<(std::ostream& os, const Event& e)
	{
		return os << e.toString();
	}
}