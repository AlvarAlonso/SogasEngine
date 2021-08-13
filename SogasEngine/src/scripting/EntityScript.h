#pragma once

#include "defines.h"
#include "LuaStateManager.h"

namespace Sogas
{
	class SGS EntityScript
	{
	private:
		LuaPlus::LuaObject m_startFunction;
		LuaPlus::LuaObject m_updateFunction;
		LuaPlus::LuaObject m_destroyFunction;
		LuaPlus::LuaObject m_self;

		std::string m_filepath;

	public:
		EntityScript() = default;

		static void registerEntityScript();
		static void initializeScriptClasses();

		void createFromScript(const std::string& scriptClass);

		void start();
		void update();
		void onDestroy();

	private:
		void createScript(LuaPlus::LuaObject constructionData, LuaPlus::LuaObject scriptClass);

		static LuaPlus::LuaObject createFromScriptData(LuaPlus::LuaObject self, LuaPlus::LuaObject constructionData, LuaPlus::LuaObject scriptClass);
		bool populateDataFromScript(LuaPlus::LuaObject scriptClass, LuaPlus::LuaObject constructionData);
		
		// for debugging
		i32 m_id = 0;
		bool m_firstUpdate = false;
	};
}
