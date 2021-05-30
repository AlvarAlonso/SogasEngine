#include "window.h"

#include "core/logger.h"
#include "core/assertions.h"

#include "events/appEvent.h"
#include "events/keyEvent.h"

static b8 s_glfwInitialized = false;

Window* Window::create(const windowProperties& properties)
{
	return new Window(properties);
}

Window::Window(const windowProperties& properties)
{
	init(properties);
}

Window::~Window() {}

b8 Window::init(const windowProperties& properties)
{
	m_data.title = properties.m_title;
	m_data.width = properties.m_width;
	m_data.height = properties.m_height;

	SGSINFO("Creating window %s [%i, %i].", properties.m_title, properties.m_width, properties.m_height);

	if (!s_glfwInitialized)
	{
		i32 success = glfwInit();
		SGSASSERT(success);

		s_glfwInitialized = true;
	}

	m_glfwWindow = glfwCreateWindow((u32)properties.m_width, (u32)properties.m_height, properties.m_title, NULL, NULL);
	if (!m_glfwWindow)
	{
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(m_glfwWindow);
	glfwSetWindowUserPointer(m_glfwWindow, &m_data);
	glfwSetWindowSizeCallback(m_glfwWindow, [](GLFWwindow* window, i32 width, i32 height) 
	{
		windowData& data = *(windowData*)glfwGetWindowUserPointer(window);
		data.width = width;
		data.height = height;

		windowResizeEvent event(width, height);
		data.eventCallback(event);
	});

	glfwSetWindowCloseCallback(m_glfwWindow, [](GLFWwindow* window) 
	{
		windowData& data = *(windowData*)glfwGetWindowUserPointer(window);
		windowCloseEvent event;
		data.eventCallback(event);
	});

	glfwSetKeyCallback(m_glfwWindow, [](GLFWwindow* window, i32 keycode, i32 scancode, i32 action, i32 mods) 
	{
		windowData& data = *(windowData*)glfwGetWindowUserPointer(window);
		
		switch (action)
		{
			case GLFW_PRESS: {
				KeyPressedEvent event(keycode, 0);
				data.eventCallback(event);
				break;
			}
			case GLFW_RELEASE:
			{
				KeyReleasedEvent event(keycode);
				data.eventCallback(event);
				break;
			}
			case GLFW_REPEAT:
			{
				//TODO: one repetition for now
				KeyPressedEvent event(keycode, 1);
				data.eventCallback(event);
				break;
			}
		}
	});


	return true;
}

void Window::shutdown()
{
	glfwDestroyWindow(m_glfwWindow);
}

void Window::onUpdate()
{
	glfwPollEvents();
	glfwSwapBuffers(m_glfwWindow);
}