#pragma once

#include "defines.h"
#include "events/event.h"

class SGS Layer
{
public:
	Layer(const std::string& name = "Layer");
	virtual ~Layer();

	virtual void onAttach() {}
	virtual void onDetach() {}
	virtual void onUpdate(f32 dt) {}
	virtual void onEvent(Event& event) {}
	virtual void onImguiRender() {}

	inline const std::string& getName() const { return m_debugName; }
protected:
	std::string m_debugName;
};