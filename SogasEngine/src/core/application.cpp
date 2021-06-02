#include "application.h"

#include "renderer/renderer.h"
#include "renderer/vertexArray.h"
#include "renderer/shader.h"
#include "platform/OpenGL/openGLShader.h"
#include "logger.h"

static Application* s_application = nullptr;

Application* Application::getInstance()
{
	if (s_application == nullptr)
	{
		s_application = new Application();
	}
	else
	{
		SGSWARN("Application constructor has been called twice!");
		s_application = this;
	}
	return s_application;
}

b8 Application::create()
{
	if (!glfwInit())
	{
		SGSFATAL("GLFW could not be initialised!");
		return -1;
	}

	m_window = glfwCreateWindow(640, 480, "Sogas Engine", NULL, NULL);
	if (!m_window)
	{
		glfwTerminate();
		SGSFATAL("Window was not properly created!");
		return -1;
	}

	glfwMakeContextCurrent(m_window);

	SGSFATAL("A test message: %f", 3.14f);
	SGSERROR("A test message: %f", 3.14f);
	SGSWARN("A test message: %f", 3.14f);
	SGSINFO("A test message: %f", 3.14f);
	SGSDEBUG("A test message: %f", 3.14f);
	SGSTRACE("A test message: %f", 3.14f);

	if(glewInit() != GLEW_OK)
	{
		SGSFATAL("Failed to link GLEW against OpenGL context!");
	}

	// creation of example render primitives
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

	return true;
}

void Application::run()
{
	// TODO: There are openGL errors not being handled. We have to implement a generalized error checking method for openGL calls

	// simple triangle program hardcoded using the opengl abstraction
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

	// main loop
	while (!glfwWindowShouldClose(m_window))
	{
		Renderer::clear();

		m_shader->bind();
		std::dynamic_pointer_cast<OpenGLShader>(m_shader)->setUniform("u_color", 1.0f);

		Renderer::drawIndexed(m_vertexArray);

		glfwSwapBuffers(m_window);

		glfwPollEvents();
	}
}

void Application::shutdown()
{
	glfwTerminate();
	delete this;
}