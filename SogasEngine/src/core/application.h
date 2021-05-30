#pragma once

#include "defines.h"
#include "GLEW/glew-2.1.0/include/GL/glew.h"
#include "GLFW/glfw3.h"
#include <stdio.h>
#include <memory>

#include "window.h"
#include "events/appEvent.h"
#include "events/keyEvent.h"

class SGS Application
{
public:
	Application();
	~Application() {};

	Application* getInstance();

	b8 m_isSuspended;
	i16 m_width;
	i16 m_height;

	b8 create();
	void run();
	void onEvent(Event& e);
	void shutdown();

private:
	b8 onWindowClosed(windowCloseEvent& e);
	b8 onWindowResize(windowResizeEvent& e);

	std::unique_ptr<Window> m_window;
	b8 m_running = true;
	b8 m_minimized = false;
};

Application* createApplication();