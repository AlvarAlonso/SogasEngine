#pragma once

#include "LuaPlus.h"

#include <string>

namespace Sogas 
{
	class SGS LuaStateManager
	{
	private:
		static LuaStateManager* s_pInstance;
		LuaPlus::LuaState* m_pLuaState;

	public:
		static bool create();
		static void destroy();
		static LuaStateManager* GET();

		void executeFile(const std::string& filename);
		void executeString(const std::string& string);

		LuaPlus::LuaObject getGlobals();
		LuaPlus::LuaState* getLuaState() const;

	private:
		explicit LuaStateManager();
		~LuaStateManager() = default;
	};
}


