#pragma once

#include "defines.h"

#include "core/camera.h"

#include "events/appEvent.h"
#include "events/keyEvent.h"
#include "events/mouseEvent.h"

class SGS CameraController
{
public:
	CameraController(std::shared_ptr<Camera> camera);

	void onUpdate(f32 dt);
	void onEvent(Event& e);

	inline void setViewportSize(const f32 width, const f32 height) { m_viewportSize.x = width, m_viewportSize.y = height; }

private:

	bool onMouseButtonPressed(MouseButtonPressedEvent& e);
	bool onMouseButtonReleased(MouseButtonReleasedEvent& e);
	bool onMouseMoved(MouseMoveEvent& e);

	std::shared_ptr<Camera> m_camera = nullptr;

	bool m_rotation		= true;
	f32 m_speed			= 0.1f;
	f32 m_sensitivity	= 10.0f;
	glm::vec2 m_viewportSize{ 0.0f, 0.0f };
	glm::vec2 m_mousePosition{ 0.0f, 0.0f };
};