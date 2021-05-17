#include "application.h"
#include <iostream>

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

	if(glewInit() != GLEW_OK)
	{
		logOutput(LOG_LEVEL_FATAL, "Failed to link GLEW against OpenGL context!");
	}

	return true;
}

void Application::run()
{
	// main loop
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