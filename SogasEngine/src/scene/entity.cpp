#include "sgspch.h"

#include "entity.h"
#include "core/application.h"

#include "imgui.h"
#include <ImGuizmo.h>

#include <glm/gtc/type_ptr.hpp>
#include "entityComponent.h"

#include "core/logger.h"
#include "core/assertions.h"

namespace Sogas 
{
	Entity::Entity(EntityId id)
	{
		m_id = id;
		m_type = "unknown";
		m_name = "entity";
	}

	Entity::~Entity(void)
	{
		//SGSINFO("Destroying actor %i", m_id);
	}

	bool Entity::init()
	{
		return true;
	}

	void Entity::postInit()
	{
		for (EntityComponentsMap::iterator it = m_components.begin(); it != m_components.end(); it++)
		{
			it->second->postInit();
		}
	}

	void Entity::destroy()
	{
		m_components.clear();
	}

	void Entity::update(f32 dt)
	{
		for (EntityComponentsMap::iterator it = m_components.begin(); it != m_components.end(); it++)
		{
			it->second->update(dt);
		}
	}

	void Entity::addComponent(StrongEntityComponentPtr pComponent)
	{
		std::pair<EntityComponentsMap::iterator, bool> success = m_components.insert(std::make_pair(pComponent->getId(), pComponent));
		SGSASSERT(success.second);
	}

	void Entity::removeComponent(const char* componentName)
	{
		ComponentId componentId = EntityComponent::getIdFromName(componentName);
		EntityComponentsMap::iterator it = m_components.find(componentId);
		if (it != m_components.end())
			m_components.erase(it);
	}
}
