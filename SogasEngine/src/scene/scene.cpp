
#include "sgspch.h"
#include "scene.h"

#include "transformComponent.h"
#include "renderComponent.h"

namespace Sogas
{
	Scene::Scene()
	{
		m_pEntityFactory.reset();
		m_pEntityFactory = std::make_unique<EntityFactory>();
	}

	Scene::~Scene()
	{
		
	}

	StrongEntityPtr Scene::createEntity(const std::string& name)
	{
		StrongEntityPtr entity = m_pEntityFactory->createEntity(name.c_str());
		m_entities.push_back(entity);
		return entity;
	}

	void Scene::destroyEntity(EntityId entity)
	{
		// TODO: Optimize insertion/deletion of entities
		int index = 0;
		for(auto& currentEntity : m_entities)
		{
			if (currentEntity->getId() == entity)
			{
				currentEntity->destroy();
				currentEntity = nullptr;
				m_entities.erase(m_entities.begin() + index);
				break; // break loop after finding the entity to be deleted
			}

			index++;
		}

		// TODO: some way to destroy an entity deleting all the references to it automatically
	}

	void Scene::destroy()
	{
		for (auto& entity : m_entities)
		{
			destroyEntity(*entity);
		}
	}

	// TODO: scene onUpdate should update and submit commands to the renderer, thus the renderer needs to have access to the shader
	// and textures to bind them when updating/rendering
	void Scene::onUpdate(f32 dt)
	{

		//for (auto& entity : m_entities)
		//{
		//	auto tComponent = entity->getComponent<TransformComponent>(TransformComponent::s_name);
		//}

	}

	void Scene::addComponent(StrongEntityPtr entity, const char* componentName)
	{
		if (!componentName)
		{
			SGSWARN("No valid component name given.");
			return;
		}

		// TODO: name from type may not be the same as stored in static s_name
		// Should find a more robust way to create a specific component type
		entity->addComponent(m_pEntityFactory->createComponent(componentName));
	}

	std::vector<StrongEntityPtr> Scene::getByComponent(const char* name)
	{
		std::vector<StrongEntityPtr> returnVector{};

		for (auto& entity : m_entities)
		{
			if (entity->has(EntityComponent::getIdFromName(name)))
			{
				returnVector.push_back(entity);
			}
		}
		
		return returnVector;
	}
}