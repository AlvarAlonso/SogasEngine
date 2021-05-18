#include "application.h"
#include "logger.h"

static Application* s_application = nullptr;

Application::Application() {}

Application::~Application() {}

Application* Application::getInstance()
{
	if (s_application == nullptr)
	{
		s_application = new Application();
	}
	else
	{
		// TODO: Add warning the constructor has been called twice
		s_application = this;
	}
	return s_application;
}

b8 Application::create()
{
	if (!glfwInit())
	{
		// TODO: return a fatal error because glfw could not be initialised
		return -1;
	}

	m_window = glfwCreateWindow(640, 480, "Sogas Engine", NULL, NULL);
	if (!m_window)
	{
		glfwTerminate();
		// TODO: return a fatal error because window was not created properly
		return -1;
	}

	glfwMakeContextCurrent(m_window);

	SGSFATAL("A test message: %f", 3.14f);
	SGSERROR("A test message: %f", 3.14f);
	SGSWARN("A test message: %f", 3.14f);
	SGSINFO("A test message: %f", 3.14f);
	SGSDEBUG("A test message: %f", 3.14f);
	SGSTRACE("A test message: %f", 3.14f);

	return true;
}

void Application::run()
{
	while (!glfwWindowShouldClose(m_window))
	{
		glfwPollEvents();
	}
}

void Application::shutdown()
{
	glfwTerminate();
	delete this;
}