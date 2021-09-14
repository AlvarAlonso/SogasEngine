#pragma once

#include "defines.h"

#include "core/layer.h"

#include "events/appEvent.h"
#include "events/keyEvent.h"
#include "events/mouseEvent.h"

namespace Sogas 
{
	class SGS ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void onAttach() override;
		virtual void onDetach() override;
		virtual void onEvent(Event& e) override;

		void blockEvents(bool block) { m_blockEvents = block; }

		void begin();
		void end();
	
	private:
		void setDarkTheme();

	private:
		bool m_blockEvents;
		f32 m_time = 0.0f;
	};
}