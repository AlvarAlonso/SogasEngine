#pragma once

#include "defines.h"

#include "core/layer.h"
#include "events/appEvent.h"
#include "events/keyEvent.h"
#include "events/mouseEvent.h"

class SGS ImGuiLayer : public Layer
{
public:
	ImGuiLayer();
	~ImGuiLayer();

	void onAttach();
	void onDetach();

	void onUpdate();
	void onEvent(Event& event);

private:
	bool onMouseButtonPressedEvent(MouseButtonPressedEvent& e);
	bool onMouseButtonReleasedEvent(MouseButtonReleasedEvent& e);
	bool onMouseMovedEvent(MouseMoveEvent& e);
	bool onMouseScrolledEvent(MouseScrolledEvent& e);
	bool onWindowResized(windowResizeEvent& e);
	bool onKeyTypedEvent(KeyTypedEvent& e);
	bool onKeyReleased(KeyReleasedEvent& e);
	bool onKeyPressed(KeyPressedEvent& e);

	f32 m_time = 0.0f;
};