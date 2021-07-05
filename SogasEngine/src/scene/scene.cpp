
#include "sgspch.h"
#include "scene.h"

#include "transformComponent.h"
#include "renderComponent.h"

#include "nlohmann/json.hpp"

using json = nlohmann::json;

namespace Sogas
{
	Scene::Scene()
	{
		m_pEntityFactory.reset();
		m_pEntityFactory = std::make_unique<EntityFactory>();
	}

	Scene::Scene(const char* filename)
	{

		std::ifstream file(filename);
		json j;
		file >> j;
		
		std::cout << j;

		m_pEntityFactory.reset();
		m_pEntityFactory = std::make_unique<EntityFactory>();

		if (j.contains("Scene"))
		{
			auto scene = j["Scene"];
			if (scene.contains("Entities")) {
				auto entities = scene["Entities"];

				for (auto entity : entities)
				{
					std::string name = entity["Name"].get<std::string>();
					// TODO: pass the entity json to the function
					auto ent = m_pEntityFactory->createEntity(name.c_str());

					if (entity.contains(TransformComponent::s_name) && !entity[TransformComponent::s_name].is_null())
					{
						StrongEntityComponentPtr component = m_pEntityFactory->createComponent(TransformComponent::s_name);
						// TODO: add transform
					}
					if (entity.contains(RenderComponent::s_name) && !entity[RenderComponent::s_name].is_null())
					{

						ent->addComponent(m_pEntityFactory->createComponent(RenderComponent::s_name));

						auto jsonComponent = entity[RenderComponent::s_name];
						std::shared_ptr<RenderComponent> component = makeStrongPtr(ent->getComponent<RenderComponent>(RenderComponent::s_name));

						if (jsonComponent.contains("Mesh") && !jsonComponent["Mesh"].is_null())
						{
							component->setMesh(jsonComponent["Mesh"].get<std::string>().c_str());
						}
						if (jsonComponent.contains("Material") && !jsonComponent["Material"].is_null())
						{
							// TODO: add material
						}

					}
					m_entities.push_back(ent);
				}

			}
		}
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

	void Scene::destroy()
	{
		for (auto& entity : m_entities)
		{
			DestroyEntity(entity);
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