#include "sgspch.h"

#include "LuaScriptAPI.h"

namespace Sogas { namespace LuaScriptAPI
{
	std::shared_ptr<Scene> LuaScriptContext::m_pScene = nullptr;

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

	LuaPlus::LuaObject getTransform(LuaPlus::LuaObject m_self)
	{
		EntityId entityID = m_self.GetByName("gameObject").GetInteger();

		LuaPlus::LuaObject result = LuaScriptContext::getContext()->findEntityById(entityID)->
			getComponent<TransformComponent>().lock()->toLuaObject(m_self);
			
		return result;
	}
} }