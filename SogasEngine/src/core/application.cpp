#include "application.h"

#include "renderer/renderer.h"
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