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
#include "imgui/imguiLayer.h"

#include "imgui.h"

class Shader;
class VertexArray;

class SGS Application
{
public:
	Application();
	~Application() {};

	static Application* getInstance() { return s_application; };
	static ImGuiContext* getImguiContext() { return s_imguiContext; };

	bool m_isSuspended;
	i16 m_width;
	i16 m_height;

	// functions
	void run();
	void close();
	void shutdown();

	void onEvent(Event& e);
	void pushLayer(Layer* layer);
	void pushOverlay(Layer* layer);

	ImGuiLayer* getImGuiLayer() { return m_imguiLayer; }
	inline Window& getWindow() { return *m_window; }

	inline void showCursor() { glfwSetInputMode(m_window.get()->getNativeWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL); }
	inline void hideCursor() { glfwSetInputMode(m_window.get()->getNativeWindow(), GLFW_CURSOR, GLFW_CURSOR_HIDDEN); }

	static ImGuiContext* s_imguiContext;
private:
	bool onWindowClosed(windowCloseEvent& e);
	bool onWindowResize(windowResizeEvent& e);

	static Application* s_application;
	std::unique_ptr<Window> m_window;
	ImGuiLayer* m_imguiLayer;
	bool m_running = true;
	bool m_minimized = false;

	LayerStack m_layerStack;
};

Application* createApplication();