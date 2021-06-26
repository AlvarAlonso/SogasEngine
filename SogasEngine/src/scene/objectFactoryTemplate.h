#pragma once

#include "defines.h"

#include <map>

namespace Sogas
{
	template<class BaseType, class SubType>
	BaseType* genericObjectCreationFunction(void) { return new SubType; }

	template<class BaseClass, class idType>
	class ObjectFactory
	{
	public:
		template<class subClass>
		bool registerComponent(idType id)
		{
			auto findIt = m_creationFunctions.find(id);
			if (findIt == m_creationFunctions.end())
			{
				m_creationFunctions[id] = &genericObjectCreationFunction<BaseClass, subClass>;
				return true;
			}
			return false;
		}

		BaseClass* create(idType id)
		{
			auto findIt = m_creationFunctions.find(id);
			if (findIt != m_creationFunctions.end())
			{
				ObjectCreationFunction pFunc = findIt->second;
				return pFunc();
			}
			return nullptr;
		}

	private:
		// Define a pointer function to the creation function of the object
		typedef BaseClass* (*ObjectCreationFunction)(void);
		std::map<idType, ObjectCreationFunction> m_creationFunctions;
	};
}