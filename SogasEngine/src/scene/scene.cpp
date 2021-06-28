
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

	void Scene::DestroyEntity(StrongEntityPtr entity)
	{
		entity->destroy();
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
}