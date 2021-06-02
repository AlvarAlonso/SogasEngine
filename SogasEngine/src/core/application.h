#pragma once

#include "defines.h"
#include "GLEW/glew-2.1.0/include/GL/glew.h"
#include "GLFW/glfw3.h"
#include <memory>

class Shader;
class VertexArray;

class SGS Application
{
public:
	Application() {};
	~Application() {};

	Application* getInstance();

	b8 m_isRunning;
	b8 m_isSuspended;
	i16 m_width;
	i16 m_height;
	GLFWwindow* m_window;

	// render primitives

	std::shared_ptr<Shader> m_shader;
	std::shared_ptr<VertexArray> m_vertexArray;

	// functions

	b8 create();
	void run();
	void shutdown();
};

Application* createApplication();