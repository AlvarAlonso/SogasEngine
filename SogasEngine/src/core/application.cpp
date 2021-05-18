#include <iostream>

#include "application.h"
#include "../renderer/renderer.h"

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
	while (!glfwWindowShouldClose(m_window))
	{
		renderer.clear();

		shader.setUniform("u_color", 1.0f);
		renderer.draw(vertexArray, indexBuffer, shader);

		glfwSwapBuffers(m_window);

		glfwPollEvents();
	}
}

void Application::shutdown()
{
	glfwTerminate();
	delete this;
}