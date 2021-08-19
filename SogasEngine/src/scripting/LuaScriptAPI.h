#pragma once

#include "defines.h"
#include "core/input.h"
#include "LuaPlus.h"

namespace Sogas { namespace LuaScriptAPI 
{
	bool isKeyPressed(i32 keyCode)
	{
		return Input::isKeyPressed(keyCode);
	}

	bool isMouseButtonPressed(i32 button)
	{
		return Input::isMouseButtonPressed(button);
	}

	bool isMouseButtonReleased(i32 button)
	{
		return Input::isMouseButtonReleased(button);
	}

	/*
	glm::vec2 getMousePosition()
	{
		return Input::getMousePosition();
	}
	*/

	void setMousePosition(f32 x, f32 y)
	{
		Input::setMousePosition(x, y);
	}

	void centerMouse()
	{
		Input::centerMouse();
	}

	/*
	LuaPlus::LuaObject getTransform(LuaPlus::LuaObject m_self)
	{

	}
	*/
} }