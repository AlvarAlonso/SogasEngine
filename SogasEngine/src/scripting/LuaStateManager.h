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

		void doFile(const std::string& filename);
		void doString(const std::string& string);

		LuaPlus::LuaObject getGlobals();
		LuaPlus::LuaState* getLuaState() const;

	private:
		bool init();
		void executeFile(const char* filename);
		void executeString(const char* string);

		explicit LuaStateManager() = default;
		~LuaStateManager() = default;
	};
}


