#include "sgspch.h"
#include "EntityScript.h"
#include "LuaStateManager.h"
#include "core/assertions.h"
#include "core/logger.h"

#include "imgui.h"

namespace Sogas
{
	void EntityScript::initializeScriptClasses()
	{
		// TODO:
		// This function should loop over all the .lua script files and execute 
		// them to define the classes we want our script to be instatiated from.
		LuaStateManager::GET()->doFile("../SogasEngine/resources/testScript.lua");
	}

	void EntityScript::registerEntityScript()
	{
		LuaPlus::LuaObject metaTableObject = LuaStateManager::GET()->getGlobals().CreateTable("ENTITY_SCRIPT");
		metaTableObject.SetObject("__index", metaTableObject);
		metaTableObject.SetObject("base", metaTableObject);
		metaTableObject.SetBoolean("cpp", true);
	}

	void EntityScript::createFromScript(const std::string& scriptClassName)
	{
		m_filepath = scriptClassName;
		m_filepath.append(".lua");

		LuaPlus::LuaObject luaGlobals = LuaStateManager::GET()->getGlobals();
		LuaPlus::LuaObject scriptClass = luaGlobals.GetByName(scriptClassName.c_str());

		if (!scriptClass.IsTable())
			SGSERROR("A Lua script must be a table!");

		createScript(scriptClass);
	}

	void EntityScript::start()
	{
		if (!m_startFunction.IsNil())
		{
			LuaPlus::LuaFunction<i32> func(m_startFunction);
			func(m_self);
		}
	}

	void EntityScript::update()
	{
		static i32 id = 0;

		if(!m_firstUpdate)
		{
			m_id = id;
			id++;
			m_firstUpdate = true;

			SGSINFO("[%i] first update!", m_id);
		}

		LuaPlus::LuaFunction<i32> func(m_updateFunction);
		func(m_self);
	}

	void EntityScript::onDestroy()
	{
		if(!m_destroyFunction.IsNil())
		{
			LuaPlus::LuaFunction<i32> func(m_destroyFunction);
			func(m_self);
		}
	}

	// TODO: refactor making it static and using smart pointers
	void EntityScript::createScript(LuaPlus::LuaObject scriptClass)
	{
		m_self.AssignNewTable(LuaStateManager::GET()->getLuaState());
		if(populateDataFromScript(scriptClass))
		{
			m_self.SetLightUserdata("__object", this);
			m_self.SetObject("__index", scriptClass);
			m_self.SetMetatable(m_self);
			i32 x = scriptClass.GetByName("x").ToInteger();
		}
		else
		{
			m_self.AssignNil(LuaStateManager::GET()->getLuaState());
			delete this;
		}
	}

	bool EntityScript::populateDataFromScript(LuaPlus::LuaObject scriptClass)
	{
		if(scriptClass.IsTable())
		{
			// Start() function
			LuaPlus::LuaObject currentFunction = scriptClass.GetByName("Start");
			if(currentFunction.IsFunction())
			{
				m_startFunction = currentFunction;
			}
			
			// Update() function
			currentFunction = scriptClass.GetByName("Update");
			if(currentFunction.IsFunction())
			{
				m_updateFunction = currentFunction;
			}
			else
			{
				SGSERROR("The script must have an Update() function! Update function was: %s", std::string(currentFunction.TypeName()));
				return false;
			}

			// OnDestroy() function
			currentFunction = scriptClass.GetByName("OnDestroy");
			if (currentFunction.IsFunction())
				m_destroyFunction = currentFunction;

			// script class attributes
			for (LuaPlus::LuaTableIterator it(scriptClass); it; it.Next())
			{
				if (it.GetKey().IsFunction())
					continue;

				const char* key = it.GetKey().GetString();
				LuaPlus::LuaObject val = it.GetValue();

				m_self.SetObject(key, val);

				EntityScriptVariable scriptVariable;

				if (val.IsInteger())
				{
					scriptVariable.type = VariableType::INTEGER;
					scriptVariable.value = (i32)val.GetInteger();
					m_scriptVariables[std::string(key)] = scriptVariable;
				}
				else if (val.IsNumber())
				{
					scriptVariable.type = VariableType::FLOAT;
					scriptVariable.value = (f32)val.GetFloat();
					m_scriptVariables[std::string(key)] = scriptVariable;
				}
				else if (val.IsString())
				{
					scriptVariable.type = VariableType::STRING;
					scriptVariable.value = (std::string)val.GetString();
					m_scriptVariables[std::string(key)] = scriptVariable;
				}
				else if (val.IsBoolean())
				{
					scriptVariable.type = VariableType::BOOLEAN;
					scriptVariable.value = (bool)val.GetBoolean();
					m_scriptVariables[std::string(key)] = scriptVariable;
				}
			}
		}
		else
		{
			SGSERROR("scriptClass must be a table when populating the script data.");
		}

		return true;
	}
}