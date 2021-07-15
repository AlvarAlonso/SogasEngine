
#include "sgspch.h"

#include "defines.h"
#include "entity.h"
#include "entityFactory.h"

#include "components/renderComponent.h"
#include "components/transformComponent.h"
#include "components/cameraComponent.h"
#include "components/lightComponent.h"

namespace Sogas
{
	u32 EntityComponent::lastId = 0;
	std::unordered_map<std::string, u32> EntityComponent::m_componentTypes{};

	EntityFactory::EntityFactory(void)
	{
		m_lastEntityId = 0;
		
		m_componentFactory.registerComponent<TransformComponent>(EntityComponent::getIdFromName(TransformComponent::s_name));
		m_componentFactory.registerComponent<RenderComponent>(EntityComponent::getIdFromName(RenderComponent::s_name));
		m_componentFactory.registerComponent<CameraComponent>(EntityComponent::getIdFromName(CameraComponent::s_name));
		m_componentFactory.registerComponent<LightComponent>(EntityComponent::getIdFromName(LightComponent::s_name));
	}

	StrongEntityPtr EntityFactory::createEntity(const char* entityResource)
	{
		// No xml or json at the moment, create default entity
		
		// TODO: generate the ID with some other function
		m_lastEntityId++;

		StrongEntityPtr pEntity(new Entity(m_lastEntityId));
		if (!pEntity->init())
		{
			SGSERROR("Failed to init entity.");
			return StrongEntityPtr();
		}

		// Transform component is added by default, all entities should have a Transform component
		{
			// At the moment load just the transform component hardcoded
			StrongEntityComponentPtr pTransformComponent(createComponent(TransformComponent::s_name));
			pEntity->addComponent(pTransformComponent);
		}
		
		pEntity->postInit();

		return pEntity;
	}

	StrongEntityComponentPtr EntityFactory::createComponent(const char* name)
	{
		StrongEntityComponentPtr pComponent(m_componentFactory.create(EntityComponent::getIdFromName(name)));

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