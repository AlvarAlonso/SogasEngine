#pragma once

#include "logger.h"
#include "../external/GLEW/glew-2.1.0/include/GL/glew.h"
#include "../external/GLFW/glfw3.h"

class SGS Application
{
public:
	Application();
	~Application();

	Application* getInstance();

	b8 m_isRunning;
	b8 m_isSuspended;
	i16 m_width;
	i16 m_height;
	GLFWwindow* m_window;

	b8 create();
	void run();
	void shutdown();
};