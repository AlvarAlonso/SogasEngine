#pragma once

#include "defines.h"
#include "types.h"

#include "objectFactoryTemplate.h"

namespace Sogas
{
	class SGS EntityFactory
	{
	public:
		EntityFactory(void);
		StrongEntityPtr createEntity(const char* entityResource);

		virtual StrongEntityComponentPtr createComponent();

		//virtual ComponentId getId(void) const = 0;

	protected:
		ObjectFactory<EntityComponent, ComponentId> m_componentFactory;

	private:

		EntityId m_lastEntityId;
		EntityId getNextEntityId(void) { ++m_lastEntityId; return m_lastEntityId; }
	};
}