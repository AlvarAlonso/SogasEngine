#include "sgspch.h"
#include "EntityScript.h"
#include "LuaStateManager.h"
#include "core/assertions.h"
#include "core/logger.h"

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
		metaTableObject.RegisterDirect("Create", &EntityScript::createFromScriptData);
	}

	void EntityScript::createFromScript(const std::string& scriptClassName)
	{
		m_filepath = scriptClassName;
		m_filepath.append(".lua");
		// TODO: This is hardcoded
		LuaPlus::LuaObject luaGlobals = LuaStateManager::GET()->getGlobals();
		LuaPlus::LuaObject scriptClass = luaGlobals.GetByName(scriptClassName.c_str());
		LuaPlus::LuaObject nilValue;
		i32 frame = luaGlobals.GetByName("frame").ToInteger();
		//nilValue.SetNil(nilValue);

		if (!scriptClass.IsTable())
			SGSERROR("A Lua script must be a table!");

		createScript(nilValue, scriptClass);
	}

	void EntityScript::start()
	{
		static i32 id = 0;

		m_id = id;
		id++;

		if (!m_startFunction.IsNil())
		{
			LuaPlus::LuaFunction<i32> func(m_startFunction);
			func(m_self);
		}
	}

	void EntityScript::update()
	{
		if(!m_firstUpdate)
		{
			SGSINFO("[%i] First Update!", m_id);
			m_firstUpdate = true;
		}

		i32 currentUpdate = LuaStateManager::GET()->getGlobals().GetByName("frame").ToInteger();

		SGSINFO("[%i]: Current frame value: %i. ", m_id, currentUpdate);

		LuaPlus::LuaFunction<i32> func(m_updateFunction);
		func(m_self);

		//i32 frame = LuaStateManager::GET()->getGlobals().GetByName("frame").ToInteger();
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
	void EntityScript::createScript(LuaPlus::LuaObject constructionData, LuaPlus::LuaObject scriptClass)
	{
		m_self.AssignNewTable(LuaStateManager::GET()->getLuaState());
		if(populateDataFromScript(scriptClass, constructionData))
		{
			LuaPlus::LuaObject metaTableObject = LuaStateManager::GET()->getGlobals().Lookup("ENTITY_SCRIPT");
			SGSASSERT(!metaTableObject.IsNil());

			m_self.SetLightUserdata("__object", this);
			m_self.SetMetatable(metaTableObject);
		}
		else
		{
			m_self.AssignNil(LuaStateManager::GET()->getLuaState());
			delete this;
		}
	}

	LuaPlus::LuaObject EntityScript::createFromScriptData(LuaPlus::LuaObject self, LuaPlus::LuaObject constructionData, LuaPlus::LuaObject scriptClass)
	{
		EntityScript* pInstance = new EntityScript();

		pInstance->m_self.AssignNewTable(LuaStateManager::GET()->getLuaState());
		if(pInstance->populateDataFromScript(scriptClass, constructionData))
		{
			LuaPlus::LuaObject metaTableObject = LuaStateManager::GET()->getGlobals().Lookup("ENTITY_SCRIPT");
			SGSASSERT(!metaTableObject.IsNil());

			pInstance->m_self.SetLightUserdata("__object", pInstance);
			pInstance->m_self.SetMetatable(metaTableObject);
		}
		else
		{
			pInstance->m_self.AssignNil(LuaStateManager::GET()->getLuaState());
			delete pInstance;
		}

		return pInstance->m_self;
	}

	bool EntityScript::populateDataFromScript(LuaPlus::LuaObject scriptClass, LuaPlus::LuaObject constructionData)
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
		}
		else
		{
			SGSERROR("scriptClass must be a table when populating the script data.");
		}

		if(constructionData.IsTable())
		{
			for(LuaPlus::LuaTableIterator it(constructionData); it; it.Next())
			{
				const char* key = it.GetKey().GetString();
				LuaPlus::LuaObject val = it.GetValue();

				m_self.SetObject(key, val);
			}
		}

		return true;
	}
}