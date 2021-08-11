#include "sgspch.h"

#include "defines.h"
#include "core/utils.h"
#include "LuaStateManager.h"

LuaStateManager* LuaStateManager::GET()
{
	// TODO: assertion

	return s_pInstance;
}
