#pragma once

#include "defines.h"
#include "LuaStateManager.h"

#include <map>
#include <variant>

typedef std::map<std::string, std::variant<std::string, i32, f32, bool>> ScriptVariablesMap;

namespace Sogas
{
	class SGS EntityScript
	{
	private:
		LuaPlus::LuaObject m_startFunction;
		LuaPlus::LuaObject m_updateFunction;
		LuaPlus::LuaObject m_destroyFunction;
		LuaPlus::LuaObject m_self;

		ScriptVariablesMap m_scriptVariables;

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
		void createScript(LuaPlus::LuaObject scriptClass);

		bool populateDataFromScript(LuaPlus::LuaObject scriptClass);
		
		// for debugging
		i32 m_id = 0;
		bool m_firstUpdate = false;
	};
}
