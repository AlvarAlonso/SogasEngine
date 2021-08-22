#include "sgspch.h"

#include "entity.h"
#include "components/lightComponent.h"
#include "components/cameraComponent.h"
#include "components/renderComponent.h"
#include "components/transformComponent.h"
#include "core/application.h"

#include "imgui.h"
#include <ImGuizmo.h>

#include <glm/gtc/type_ptr.hpp>

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
		SGSINFO("Destroying entity %i", m_id);
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

	void Entity::to_json(json& j)
	{
		json transform, render, light;
		getComponent<TransformComponent>().lock()->to_json(transform);

		if (has<RenderComponent>())
			getComponent<RenderComponent>().lock()->to_json(render);

		if (has<LightComponent>())
			getComponent<LightComponent>().lock()->to_json(light);

		j["Name"] = !getName().empty() ? getName() : "Entity";
		j[TransformComponent::s_name] = transform;
		j[RenderComponent::s_name] = render;
		j[LightComponent::s_name] = light;
	}

	void Entity::from_json(const json& jsonEntity)
	{
		/*
		* Add name to the entity if one
		*/
		if (jsonEntity.contains("Name"))
		{
			setName(jsonEntity["Name"].get<std::string>());
		}

		// -----------------------
		// Check if entity contains Transform Component
		// -----------------------
		if (jsonEntity.contains(TransformComponent::s_name) && !jsonEntity[TransformComponent::s_name].is_null())
		{
			auto jsonTransform = jsonEntity[TransformComponent::s_name];
			makeStrongPtr<TransformComponent>(getComponent<TransformComponent>())->from_json(jsonTransform);
		}

		// -----------------------
		// Check if entity contains Render Component
		// -----------------------
		if (jsonEntity.contains(RenderComponent::s_name) && !jsonEntity[RenderComponent::s_name].is_null())
		{
			auto jsonComponent = jsonEntity[RenderComponent::s_name];
			makeStrongPtr(getComponent<RenderComponent>())->from_json(jsonComponent);
		}

		// -----------------------
		// Check if entity contains Light Component
		// -----------------------
		if (jsonEntity.contains(LightComponent::s_name) && !jsonEntity[LightComponent::s_name].is_null())
		{
			auto jsonLight = jsonEntity[LightComponent::s_name];
			makeStrongPtr(getComponent<LightComponent>())->from_json(jsonLight);
		}
	}

	const std::vector<StrongEntityComponentPtr>& Entity::getComponentsVector()
	{
		std::vector<StrongEntityComponentPtr> componentsVector{};
		EntityComponentsMap::iterator it;
		for (it = m_components.begin(); it != m_components.end(); it++)
		{
			componentsVector.push_back(it->second);
		}

		return componentsVector;
	}

	void Entity::addComponent(StrongEntityComponentPtr pComponent)
	{
		std::pair<EntityComponentsMap::iterator, bool> success = m_components.insert(std::make_pair(pComponent->getId(), pComponent));

		
		StrongEntityPtr pEntity;
		pEntity.reset(this);
		pComponent->setOwner(pEntity);

		pComponent->postInit();
		
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
