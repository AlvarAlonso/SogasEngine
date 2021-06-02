#include "application.h"
#include <memory>

#include "renderer/renderer.h"
#include "renderer/vertexArray.h"
#include "renderer/shader.h"
#include "platform/OpenGL/openGLShader.h"
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

	// renderer example primitive usage
	m_vertexArray.reset(VertexArray::create());

	f32 positions[9] = {
		-0.5f, -0.5f, 0.0f,
		0.0f, 0.5f, 0.0f,
		0.5f, -0.5f, 0.0f
	};

	u32 indices[3] = {
		0, 1, 2
	};

	std::shared_ptr<VertexBuffer> vertexBuffer;
	vertexBuffer.reset(VertexBuffer::create(positions, sizeof(positions)));

	VertexBufferLayout layout = { {ShaderDataType::Float3, "a_position"} };

	vertexBuffer->setLayout(layout);
	m_vertexArray->addVertexBuffer(vertexBuffer);

	std::shared_ptr<IndexBuffer> indexBuffer;
	indexBuffer.reset(IndexBuffer::create(indices, sizeof(indices) / sizeof(u32)));
	m_vertexArray->setIndexBuffer(indexBuffer);

	m_shader.reset(Shader::create("../SogasEngine/shaders/basic.shader"));

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
		Renderer::clear();

		m_shader->bind();
		std::dynamic_pointer_cast<OpenGLShader>(m_shader)->setUniform("u_color", 1.0f);

		Renderer::draw(m_vertexArray);
		//Renderer::drawIndexed(m_vertexArray);

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
