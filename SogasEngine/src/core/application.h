#pragma once

#include "defines.h"
#include "GLEW/glew-2.1.0/include/GL/glew.h"
#include "GLFW/glfw3.h"

#include <stdio.h>
#include <memory>

#include "window.h"
#include "layerStack.h"
#include "events/appEvent.h"
#include "events/keyEvent.h"

class Shader;
class VertexArray;

class SGS Application
{
public:
	Application();
	~Application() {};

	static Application* getInstance();

	bool m_isSuspended;
	i16 m_width;
	i16 m_height;

	// functions
	void run();
	void shutdown();

	void onEvent(Event& e);
	void pushLayer(Layer* layer);
	void pushOverlay(Layer* layer);

	inline Window& getWindow() { return *m_window; }

private:
	bool onWindowClosed(windowCloseEvent& e);
	bool onWindowResize(windowResizeEvent& e);

	std::unique_ptr<Window> m_window;
	bool m_running = true;
	bool m_minimized = false;

	LayerStack m_layerStack;
};

Application* createApplication();