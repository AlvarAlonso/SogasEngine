#pragma once

#include "defines.h"
#include "core/input.h"
#include "LuaPlus.h"
#include "scene/scene.h"

#include "scene/components/transformComponent.h"

namespace Sogas { namespace LuaScriptAPI 
{
	class SGS LuaScriptContext
	{
	private:
		static std::shared_ptr<Scene> m_pScene;
		
	public:
		static void setContext(std::shared_ptr<Scene> pScene) { m_pScene = pScene; }
		static std::shared_ptr<Scene> getContext() { return m_pScene; }
	}; 

	bool isKeyPressed(i32 keyCode);

	bool isMouseButtonPressed(i32 button);

	bool isMouseButtonReleased(i32 button);

	/*
	glm::vec2 getMousePosition()
	{
		return Input::getMousePosition();
	}
	*/

	void setMousePosition(f32 x, f32 y);

	void centerMouse();

	LuaPlus::LuaObject getTransform(LuaPlus::LuaObject m_self);
} }