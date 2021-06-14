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

	virtual void onAttach() override;
	virtual void onDetach() override;
	virtual void onImguiRender() override;

	void begin();
	void end();

private:

	f32 m_time = 0.0f;
};