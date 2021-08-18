#pragma once

#include "defines.h"
#include "LuaStateManager.h"

#include <map>
#include <variant>

enum class VariableType
{
	UNDEFINED = 0,
	STRING,
	INTEGER,
	FLOAT,
	BOOLEAN
};

typedef std::variant<std::string, i32, f32, bool> EntityScriptVariant;

struct EntityScriptVariable
{
	VariableType type;
	EntityScriptVariant value;
};

typedef std::map<std::string, EntityScriptVariable> ScriptVariablesMap;

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

		ScriptVariablesMap& getScriptVariables() { return m_scriptVariables; }

	private:
		void createScript(LuaPlus::LuaObject scriptClass);
		static void registerScriptFunctions();

		bool populateDataFromScript(LuaPlus::LuaObject scriptClass);

		// for debugging
		i32 m_id = 0;
		bool m_firstUpdate = false;
	};
}
