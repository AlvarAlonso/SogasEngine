
#include "sgspch.h"

#include "defines.h"
#include "entity.h"
#include "entityFactory.h"

#include "transformComponent.h"
#include "renderComponent.h"
//#include "../external/json/include/nlohmann/json.hpp"

namespace Sogas
{
	u32 EntityComponent::lastId = 0;
	std::unordered_map<const char*, u32> EntityComponent::m_componentTypes{};

	EntityFactory::EntityFactory(void)
	{
		m_lastEntityId = 0;
		
		m_componentFactory.registerComponent<TransformComponent>(EntityComponent::getIdFromName(TransformComponent::s_name));
		//m_componentFactory.registerComponent<RenderComponent>(EntityComponent::getIdFromName(RenderComponent::s_name));
	}

	StrongEntityPtr EntityFactory::createEntity(const char* entityResource)
	{
		// No xml or json at the moment, create default entity

		StrongEntityPtr pEntity(new Entity(m_lastEntityId));
		if (!pEntity->init())
		{
			SGSERROR("Failed to init entity.");
			return StrongEntityPtr();
		}

		{
			// At the moment load just the transform component hardcoded
			StrongEntityComponentPtr pTransformComponent(createComponent());
			pEntity->addComponent(pTransformComponent);
		}
		// Transform component is added by default, all entities should have a Transform component
		std::shared_ptr<TransformComponent> pTransformComponent = makeStrongPtr(pEntity->getComponent<TransformComponent>(TransformComponent::getIdFromName(TransformComponent::s_name)));
		
		pEntity->postInit();

		return pEntity;
	}

	StrongEntityComponentPtr EntityFactory::createComponent()
	{
		StrongEntityComponentPtr pComponent(m_componentFactory.create(EntityComponent::getIdFromName("TransformComponent")));

		if (pComponent)
		{
			if (!pComponent->init())
			{
				SGSERROR("Component failed to initialize!");
				return StrongEntityComponentPtr();
			}
		}
		else
		{
			SGSERROR("Could not find such component!");
			return StrongEntityComponentPtr();
		}
		// No xml or json at the moment, create default component
		return pComponent;
	}
}