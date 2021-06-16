#pragma once

#include "defines.h"
#include "GLFW/glfw3.h"
#include <functional>

#include "events/event.h"

namespace Sogas 
{
	struct windowProperties
	{
		u32 m_width;
		u32 m_height;
		const char* m_title;

		windowProperties(
			const char* title = "Sogas Engine",
			u32 width = 1280,
			u32 height = 720)
			: m_title(title), m_width(width), m_height(height)
		{
		}
	};

	class Window
	{
	public:
		using eventCallbackFunc = std::function<void(Event&)>;

		Window(const windowProperties& properties);
		~Window();

		bool init(const windowProperties& properties);
		void onUpdate();

		inline u32 getWidth() const { return m_data.width; };
		inline u32 getHeight() const { return m_data.height; };

		inline void setEventCallback(const eventCallbackFunc& callback) { m_data.eventCallback = callback; }

		inline GLFWwindow* getNativeWindow() { return m_glfwWindow; }

		static Window* create(const windowProperties& properties = windowProperties());

	private:
		void shutdown();

		GLFWwindow* m_glfwWindow;

		struct windowData
		{
			const char* title;
			u32 width, height;

			eventCallbackFunc eventCallback;
		};

		windowData m_data;
	};
}