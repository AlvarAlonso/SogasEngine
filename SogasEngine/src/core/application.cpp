#include "sgspch.h"

#include "application.h"

#include "renderer/renderer.h"
#include "renderer/vertexArray.h"
#include "renderer/shader.h"
#include "renderer/resources/texture.h"
#include "platform/OpenGL/openGLShader.h"
#include "logger.h"
#include "time.h"
#include "camera.h"
#include "scripting/LuaStateManager.h"
#include "scripting/EntityScript.h"

#include "input.h"

namespace Sogas 
{
	Application* Application::s_application = nullptr;
	ImGuiContext* Application::s_imguiContext = nullptr;

	static Camera* s_camera = nullptr;

	// TODO Redefine how the path should be used or saved
	std::vector<std::string> g_assetsPath;

	// TODO: remove test attributes
	EntityScript m_entityScript;

	Application::Application()
	{
		
		(!s_application);
		s_application = this;
		m_window = std::unique_ptr<Window>(Window::create());
		m_window->setEventCallback(BIND_EVENT_FUNC(Application::onEvent));

		m_imguiLayer = new ImGuiLayer();
		pushOverlay(m_imguiLayer);

		if (glewInit() != GLEW_OK)
		{
			SGSFATAL("Failed to link GLEW against OpenGL context!");
		}

		Texture2D::initTextureResources();

		LuaStateManager::create();
		EntityScript::registerEntityScript();

		m_entityScript.createFromScript();

		g_assetsPath = {
			"../Assets/",
			"../Assets/meshes/",
			"../Assets/textures/",
			"../Assets/scenes/",
			"../Assets/scripts/",
			"../SogasEngine/shaders/"
		};
	}

	void Application::run()
	{
		// TODO: There are openGL errors not being handled. We have to implement a generalized error checking method for openGL calls

		// main loop
		while (m_running)
		{
			Time::deltaTime = (f32)glfwGetTime() - (f32)Time::time;
			Time::time = glfwGetTime();
			if (!m_minimized)
			{
				for (Layer* layer : m_layerStack)
				{
					layer->onUpdate(Time::deltaTime);
				}

				m_imguiLayer->begin();
				for (Layer* layer : m_layerStack)
				{
					layer->onImguiRender();
				}
				m_imguiLayer->end();
			}
			m_window->onUpdate();
		}
	}

	void Application::close()
	{
		m_running = false;
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
}