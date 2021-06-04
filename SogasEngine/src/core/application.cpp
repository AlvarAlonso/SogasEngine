#include "application.h"
#include <memory>

#include "renderer/renderer.h"
#include "renderer/vertexArray.h"
#include "renderer/shader.h"
#include "platform/OpenGL/openGLShader.h"
#include "logger.h"
#include "time.h"
#include "camera.h"

#include "input.h"

static Application* s_application = nullptr;

static Camera* s_camera = nullptr;

Application::Application()
{
	m_window = std::unique_ptr<Window>(Window::create());
	m_window->setEventCallback(BIND_EVENT_FUNC(Application::onEvent));

	if(glewInit() != GLEW_OK)
	{
		SGSFATAL("Failed to link GLEW against OpenGL context!");
	}



	s_application = this;
}

Application* Application::getInstance()
{
	if (s_application == nullptr)
	{
		s_application = new Application();
	}

	return s_application;
}

void Application::run()
{
	// TODO: There are openGL errors not being handled. We have to implement a generalized error checking method for openGL calls

	// simple triangle program hardcoded using the opengl abstraction
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

	// main loop
	while (m_running)
	{
		Time::deltaTime = (f32)glfwGetTime() - (f32)Time::time;
		Time::time = glfwGetTime();

		for (Layer* layer : m_layerStack)
		{
			layer->onUpdate(Time::deltaTime);
		}

		m_window->onUpdate();
	}
}

void Application::onEvent(Event& e)
{
	EventDispatcher dispatcher(e);
	dispatcher.dispatch<windowCloseEvent>(BIND_EVENT_FUNC(Application::onWindowClosed));
	dispatcher.dispatch<windowResizeEvent>(BIND_EVENT_FUNC(Application::onWindowResize));

	// Loop in reverse since most top layer has the most priority in the queue
	for (auto it = m_layerStack.rbegin(); it != m_layerStack.rend(); it++)
	{
		if (e.handled)
			break;
		(*it)->onEvent(e);
	}
}

void Application::pushLayer(Layer* layer)
{
	m_layerStack.pushLayer(layer);
	layer->onAttach();
}

void Application::pushOverlay(Layer* layer)
{
	m_layerStack.pushOverlay(layer);
	layer->onAttach();
}

bool Application::onWindowClosed(windowCloseEvent& e)
{
	m_running = false;
	return true;
}

bool Application::onWindowResize(windowResizeEvent& e)
{

	if (e.getWidth() == 0 || e.getHeight() == 0)
	{
		m_minimized = true;
		return false;
	}

	m_minimized = false;
	glViewport(0, 0, e.getWidth(), e.getHeight());
	return false;
}

void Application::shutdown()
{
	glfwTerminate();
	delete this;
}
