#include "sgspch.h"
#include "EntityScript.h"
#include "LuaStateManager.h"
#include "core/assertions.h"
#include "core/logger.h"

namespace Sogas
{
	void EntityScript::registerEntityScript()
	{
		LuaPlus::LuaObject metaTableObject = LuaStateManager::GET()->getGlobals().CreateTable("ENTITY_SCRIPT");
		metaTableObject.SetObject("__index", metaTableObject);
		metaTableObject.SetObject("base", metaTableObject);
		metaTableObject.SetBoolean("cpp", true);
		metaTableObject.RegisterDirect("Create", &EntityScript::createFromScriptData);
	}

	void EntityScript::createFromScript()
	{
		// TODO: This is hardcoded
		LuaStateManager::GET()->doFile("../SogasEngine/resources/testScript.lua");

		LuaPlus::LuaObject luaGlobals = LuaStateManager::GET()->getGlobals();
		LuaPlus::LuaObject script = luaGlobals.GetByName("testScript");
		LuaPlus::LuaObject nilValue;
		i32 frame = luaGlobals.GetByName("frame").ToInteger();
		//nilValue.SetNil(nilValue);

		if (!script.IsTable())
			SGSERROR("A Lua script must be a table!");

		populateDataFromScript(script, nilValue);
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