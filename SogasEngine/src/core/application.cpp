#include "application.h"
#include <memory>

#include "renderer/renderer.h"
#include "logger.h"

static Application* s_application = nullptr;

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

	f32 positions[6] = {
		-0.5f, -0.5f,
		0.0f, 0.5f,
		0.5f, -0.5f
	};

	Renderer renderer;
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

	VertexArray vertexArray;
	VertexBuffer vertexBuffer((const void*)positions, 6 * sizeof(f32));
	VertexBufferLayout vertexBufferLayout;
	vertexBufferLayout.push<f32>(2);
	vertexArray.addBuffer(vertexBuffer, vertexBufferLayout);

	u32 indices[3] = {
		0, 1, 2
	};

	IndexBuffer indexBuffer(indices, 3);

	std::string path("../SogasEngine/shaders/basic.shader");
	Shader shader(path);

	vertexArray.unbind();
	vertexBuffer.unbind();
	indexBuffer.unbind();
	shader.unbind();

	// main loop
	while (m_running)
	{
		renderer.clear();

		shader.setUniform("u_color", 1.0f);
		renderer.draw(vertexArray, indexBuffer, shader);

		for (Layer* layer : m_layerStack)
		{
			layer->onUpdate();
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

	SGSINFO(e.toString().c_str());
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

b8 Application::onWindowClosed(windowCloseEvent& e)
{
	m_running = false;
	return true;
}

b8 Application::onWindowResize(windowResizeEvent& e)
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
