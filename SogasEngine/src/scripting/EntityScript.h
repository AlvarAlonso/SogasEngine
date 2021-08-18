#pragma once

#include "defines.h"
#include "LuaStateManager.h"

#include <map>
namespace Sogas
{
	enum class VariableType
	{
		UNDEFINED = 0,
		INTEGER,
		FLOAT,
		STRING,
		BOOLEAN
	};

	class SGS ScriptAttribute
	{
	public:
		VariableType type;

	private:
		// TODO: refactor; scene panel cannot have access to pAttribute
		LuaPlus::LuaObject* pAttribute;

		friend class EntityScript;

	public: 

		ScriptAttribute() { type = VariableType::UNDEFINED; pAttribute = new LuaPlus::LuaObject(); }

		i32 getInteger() { return pAttribute->GetInteger(); }

		f32 getFloat() { return pAttribute->GetFloat(); }

		std::string getString() { return pAttribute->GetString(); }

		bool getBoolean() { return pAttribute->GetBoolean(); }
	};

	// TODO: refactor; convert it to vector of ScriptAttributes. ScriptAttribute should own name and type. With the name it
	// can map to the variable in m_self and with the type can get the correct type.
	typedef std::map<std::string, ScriptAttribute> ScriptVariablesMap;

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
