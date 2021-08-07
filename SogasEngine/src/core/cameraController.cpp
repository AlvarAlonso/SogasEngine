#include "sgspch.h"

#include "cameraController.h"

#include "input.h"
#include "keyCodes.h"
#include "mouseButtonCodes.h"
#include "application.h"

#include "logger.h"
#include "assertions.h"

namespace Sogas 
{
	CameraController::CameraController(std::shared_ptr<Camera> camera = nullptr)
		: m_camera(camera)
	{
	}

	void CameraController::onUpdate(f32 dt)
	{
		m_deltaTime = dt;

		if (Input::isKeyPressed(SGS_KEY_KP_2))
		{
			m_camera->move(DOWN, dt);
		}
		if (Input::isKeyPressed(SGS_KEY_KP_4))
		{
			m_camera->move(LEFT, dt);
		}
		if (Input::isKeyPressed(SGS_KEY_KP_6))
		{
			m_camera->move(RIGHT, dt);
		}
		if (Input::isKeyPressed(SGS_KEY_KP_8))
		{
			m_camera->move(UP, dt);
		}
	}

	void CameraController::onEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.dispatch<MouseButtonPressedEvent>(BIND_EVENT_FUNC(CameraController::onMouseButtonPressed));
		dispatcher.dispatch<MouseButtonReleasedEvent>(BIND_EVENT_FUNC(CameraController::onMouseButtonReleased));
		dispatcher.dispatch<MouseMoveEvent>(BIND_EVENT_FUNC(CameraController::onMouseMoved));
		dispatcher.dispatch<MouseScrolledEvent>(BIND_EVENT_FUNC(CameraController::onMouseScrolled));
	}

	bool CameraController::onMouseButtonPressed(MouseButtonPressedEvent& e)
	{
		if (e.getMouseButton() == SGS_MOUSE_BUTTON_MIDDLE)
		{
			//Application::getInstance()->hideCursor();
		}
		SGSDEBUG("Mouse button %s pressed.", e.toString().c_str());
		return false;
	}

	bool CameraController::onMouseButtonReleased(MouseButtonReleasedEvent& e)
	{
		// Hide the cursor if using the camera
		if (e.getMouseButton() == SGS_MOUSE_BUTTON_MIDDLE)
		{
			//Application::getInstance()->showCursor();
		}
		SGSDEBUG("Mouse button %s released.", e.toString().c_str());
		return false;
	}

	bool CameraController::onMouseMoved(MouseMoveEvent& e)
	{
		glm::vec2 deltaMouse = m_mousePosition - Input::getMousePosition();
		if (Input::isMouseButtonPressed(SGS_MOUSE_BUTTON_MIDDLE) && Input::isKeyPressed(SGS_KEY_LEFT_SHIFT))
		{
			m_camera->pan(-deltaMouse.x * m_deltaTime, -deltaMouse.y * m_deltaTime);
		}
		else if (Input::isMouseButtonPressed(SGS_MOUSE_BUTTON_MIDDLE) && Input::isKeyPressed(SGS_KEY_LEFT_CONTROL))
		{
			// TODO fix zoom
			m_camera->zoom(deltaMouse.y * m_deltaTime);
		}
		else if (Input::isMouseButtonPressed(SGS_MOUSE_BUTTON_MIDDLE) && Input::isKeyPressed(SGS_KEY_LEFT_ALT))
		{
		}
		else if (Input::isMouseButtonPressed(SGS_MOUSE_BUTTON_MIDDLE))
		{
			m_camera->rotate(deltaMouse.x * m_deltaTime, deltaMouse.y * m_deltaTime);
			//m_camera->orbit(deltaMouse.x * m_deltaTime, deltaMouse.y * m_deltaTime);
			//Input::centerMouse();
		}


		m_mousePosition = Input::getMousePosition();
		//SGSDEBUG("Mouse is being moved [%f, %f]", m_mousePosition.x, m_mousePosition.y);
		return false;
	}

	bool CameraController::onMouseScrolled(MouseScrolledEvent& e)
	{
		m_camera->zoom(e.getOffsetY() * m_deltaTime);
		//SGSDEBUG(e.toString().c_str());
		return false;
	}
}