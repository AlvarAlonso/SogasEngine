#include "sgspch.h"

#include "defines.h"
#include "core/utils.h"
#include "LuaStateManager.h"

namespace Sogas
{
	LuaStateManager* LuaStateManager::s_pInstance = nullptr;

	LuaStateManager* LuaStateManager::GET()
	{
		// TODO: assertion

		return s_pInstance;
	}
}