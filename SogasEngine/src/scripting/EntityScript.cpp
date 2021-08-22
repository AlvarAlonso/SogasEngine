#include "sgspch.h"
#include "EntityScript.h"
#include "LuaStateManager.h"
#include "LuaScriptAPI.h"
#include "core/assertions.h"
#include "core/logger.h"
#include "scene/components/entityScriptComponent.h"
#include "scene/entity.h"

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
		registerScriptFunctions();
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

	void EntityScript::setOwner(EntityScriptComponent* pOwner)
	{
		m_pOwner = pOwner;

		StrongEntityPtr ownerEntity = m_pOwner->getOwner();
		m_self.SetNumber("gameObject", ownerEntity->getId());

		ScriptAttribute entityID;
		entityID.type = VariableType::INTEGER;
		entityID.pAttribute->AssignInteger(LuaStateManager::GET()->getLuaState(), m_self.GetByName("gameObject").GetInteger());
		m_scriptVariables[std::string("gameObject")] = entityID;
	}

	void EntityScript::start()
	{
		if (!m_startFunction.IsNil())
		{
			LuaPlus::LuaFunction<i32> func(m_startFunction);
			func(m_self);
		}
	}

	void EntityScript::update(f32 dt)
	{
		static i32 id = 0;

		if(!m_firstUpdate)
		{
			m_id = id;
			id++;
			m_firstUpdate = true;

			SGSINFO("[%i] first update!", m_id);
		}

		f32 x = 0, y = 0, z = 0;

		if(m_self.GetByName("transform").GetByName("x").IsNumber())
			x = m_self.GetByName("transform").GetByName("x").GetFloat();

		if (m_self.GetByName("transform").GetByName("y").IsNumber())
			y = m_self.GetByName("transform").GetByName("y").GetFloat();

		if (m_self.GetByName("transform").GetByName("z").IsNumber())
			z = m_self.GetByName("transform").GetByName("z").GetFloat();

		SGSINFO("ID [%i] transform: x: %f, y: %f, z: %f", m_id, x, y, z);



		LuaPlus::LuaObject frameDeltaTime;
		frameDeltaTime.AssignNumber(LuaStateManager::GET()->getLuaState(), dt);

		LuaPlus::LuaFunction<i32> func(m_updateFunction);
		func(m_self, frameDeltaTime);
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
		}
		else
		{
			m_self.AssignNil(LuaStateManager::GET()->getLuaState());
			delete this;
		}
	}

	void EntityScript::registerScriptFunctions()
	{
		// TODO: Each function has to be registered to the proper class, not to the global namespace
		LuaStateManager::GET()->getGlobals().RegisterDirect("IsKeyPressed", &LuaScriptAPI::isKeyPressed);
		LuaStateManager::GET()->getGlobals().RegisterDirect("IsMouseButtonPressed", &LuaScriptAPI::isMouseButtonPressed);
		LuaStateManager::GET()->getGlobals().RegisterDirect("IsMouseButtonReleased", &LuaScriptAPI::isMouseButtonReleased);
		//LuaStateManager::GET()->getGlobals().RegisterDirect("GetMousePosition", &LuaScriptAPI::getMousePosition);
		LuaStateManager::GET()->getGlobals().RegisterDirect("SetMousePosition", &LuaScriptAPI::setMousePosition);
		LuaStateManager::GET()->getGlobals().RegisterDirect("CenterMouse", &LuaScriptAPI::centerMouse);
		LuaStateManager::GET()->getGlobals().RegisterDirect("GetTransform", &LuaScriptAPI::getTransform);
		LuaStateManager::GET()->getGlobals().RegisterDirect("SetTransform", &LuaScriptAPI::setTransform);

		//metaTableObject.RegisterDirect("isKeyPressed", &LuaScriptAPI::isKeyPressed);
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
				if (it.GetValue().IsFunction())
					continue;

				const char* key = it.GetKey().GetString();
				LuaPlus::LuaObject val = it.GetValue();

				m_self.SetObject(key, val);
				
				ScriptAttribute attribute;

				if (val.IsNumber())
				{
					attribute.type = VariableType::FLOAT;
					attribute.pAttribute->AssignNumber(LuaStateManager::GET()->getLuaState(), val.GetNumber());

					// TODO: Check if it is a float or an integer
					m_scriptVariables[std::string(key)] = attribute;
				}
				else if (val.IsInteger())
				{
					attribute.type = VariableType::INTEGER;
					attribute.pAttribute->AssignInteger(LuaStateManager::GET()->getLuaState(), val.GetInteger());
					m_scriptVariables[std::string(key)] = attribute;
				}
				else if (val.IsString())
				{
					attribute.type = VariableType::STRING;
					attribute.pAttribute->AssignString(LuaStateManager::GET()->getLuaState(), val.GetString());
					m_scriptVariables[std::string(key)] = attribute;
				}
				else if (val.IsBoolean())
				{
					attribute.type = VariableType::BOOLEAN;
					attribute.pAttribute->AssignBoolean(LuaStateManager::GET()->getLuaState(), val.GetBoolean());
					m_scriptVariables[std::string(key)] = attribute;
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