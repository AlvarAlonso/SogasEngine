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
		LuaPlus::LuaObject m_self;

	public:
		static void registerEntityScript();

	private:
		static LuaPlus::LuaObject createFromScriptData(LuaPlus::LuaObject self, LuaPlus::LuaObject constructionData, LuaPlus::LuaObject scriptClass);
		bool populateDataFromScript(LuaPlus::LuaObject scriptClass, LuaPlus::LuaObject constructionData);
	};
}
