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
	b8 onMouseButtonPressedEvent(MouseButtonPressedEvent& e);
	b8 onMouseButtonReleasedEvent(MouseButtonReleasedEvent& e);
	b8 onMouseMovedEvent(MouseMoveEvent& e);
	b8 onMouseScrolledEvent(MouseScrolledEvent& e);
	b8 onWindowResized(windowResizeEvent& e);
	b8 onKeyTypedEvent(KeyTypedEvent& e);
	b8 onKeyReleased(KeyReleasedEvent& e);
	b8 onKeyPressed(KeyPressedEvent& e);

	f32 m_time = 0.0f;
};