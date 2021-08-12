#pragma once

#include "LuaStateManager.h"

namespace Sogas
{
	class EntityScript
	{
	private:
		LuaPlus::LuaObject m_startFunction;
		LuaPlus::LuaObject m_updateFunction;
		LuaPlus::LuaObject m_destroyFunction;
		LuaPlus::LuaObject m_script;

	public:
		static void registerEntityScript();

	private:
		static void createScriptData();
	};
}
