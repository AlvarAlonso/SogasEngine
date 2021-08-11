#include "sgspch.h"

#include "defines.h"
#include "core/utils.h"
#include "core/logger.h"
#include "LuaStateManager.h"

namespace Sogas
{
	LuaStateManager* LuaStateManager::s_pInstance = nullptr;

	bool LuaStateManager::create()
	{
		if(s_pInstance)
		{
			SGSWARN("LuaStateManager is already created!");
		}

		s_pInstance = new LuaStateManager();
		if (s_pInstance)
			return s_pInstance->init();

		return false;
	}

	void LuaStateManager::destroy()
	{
		SGSASSERT(s_pInstance);
		delete s_pInstance;
		s_pInstance = nullptr;
	}

	LuaStateManager* LuaStateManager::GET()
	{
		SGSASSERT(s_pInstance);
		return s_pInstance;
	}

	void LuaStateManager::doFile(const std::string& filename)
	{
		executeFile(filename.c_str());
	}

	void LuaStateManager::doString(const std::string& string)
	{
		executeString(string.c_str());
	}

	LuaPlus::LuaObject LuaStateManager::getGlobals()
	{
		return m_pLuaState->GetGlobals();
	}

	LuaPlus::LuaState* LuaStateManager::getLuaState() const
	{
		return m_pLuaState;
	}

	bool LuaStateManager::init()
	{
		m_pLuaState = LuaPlus::LuaState::Create(true);
		if (m_pLuaState == nullptr)
			return false;

		m_pLuaState->GetGlobals().RegisterDirect("executeFile", (*this), &LuaStateManager::executeFile);
		m_pLuaState->GetGlobals().RegisterDirect("executeString", (*this), &LuaStateManager::executeString);
		
		return true;
	}
	void LuaStateManager::executeFile(const char* filename)
	{
		i32 error = m_pLuaState->DoFile(filename);
		if (error)
			SGSERROR("LuaState error: %i", error);
	}

	void LuaStateManager::executeString(const char* string)
	{
		i32 error = m_pLuaState->DoString(string);
		if (error)
			SGSERROR("LuaState error: %i", error);
	}
}