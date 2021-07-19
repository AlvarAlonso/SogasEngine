
#include "sgspch.h"
#include "scene.h"

#include "components/transformComponent.h"
#include "components/renderComponent.h"
#include "components/lightComponent.h"
#include "components/cameraComponent.h"

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

		m_pEntityFactory.reset();
		m_pEntityFactory = std::make_unique<EntityFactory>();

		from_json(j);
	}

	Scene::~Scene()
	{
		
	}

	/*
	* TODO: Should probably receive the path to the json to deserialize, name should not be
	* added at creation. 
	* @brief Create automatically an entity to the scene
	* @param const std::string& name
	* @return StrongEntityPtr
	*/
	StrongEntityPtr Scene::createEntity(const std::string& name)
	{
		StrongEntityPtr entity = m_pEntityFactory->createEntity(name.c_str());
		m_entities.push_back(entity);
		return entity;
	}

	void Scene::destroyEntity(EntityId entityId)
	{
		// TODO: Optimize insertion/deletion of entities

		i32 index = 0;
		for(auto& currentEntity : m_entities)
		{
			if (currentEntity->getId() == entityId)
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

	// TODO: scene onUpdate should update and submit commands to the renderer, thus the renderer needs to have access to the shader
	// and textures to bind them when updating/rendering
	void Scene::onUpdate(f32 dt)
	{

		//for (auto& entity : m_entities)
		//{
		//	auto tComponent = entity->getComponent<TransformComponent>(TransformComponent::s_name);
		//}

	}

	/*
	* @brief Search for the entity with the correspondant id and returns it
	* @param EntityId
	* @return StrongEntityPtr (shared_ptr<Entity>)
	*/

	StrongEntityPtr Scene::findEntityById(EntityId entityId)
	{
		for (i32 index = 0; index < m_entities.size(); index++)
		{
			if(m_entities[index]->getId() == entityId)
			{
				return m_entities[index];
			}
		}

		return nullptr;
	}

	/*
	* @brief Serialize all data in the scene to json
	* @param json&
	* @return void
	*/
	void Scene::to_json(json& j)
	{
		json entArray = json::array();
		for (auto& entity : getEntities())
		{
			json ent;
			entity->to_json(ent);
			entArray.push_back(ent);
		}
		j = json{ {"Entities", entArray} };
	}

	/*
	* @brief Deserialize all data from a json file to a scene
	* @param const json&
	* @return void
	*/
	void Scene::from_json(const json& j)
	{
		if (j.contains("Scene"))
		{
			auto scene = j["Scene"];

			if (scene.contains("Entities"))
			{
				auto jsonEntities = scene["Entities"];
				for (auto& jsonEntity : jsonEntities)
				{
					std::string name = jsonEntity["Name"].get<std::string>();

					auto entity = m_pEntityFactory->createEntity(name.c_str());

					if(jsonEntity.contains(RenderComponent::s_name) && !jsonEntity[RenderComponent::s_name].is_null())
						entity->addComponent(m_pEntityFactory->createComponent(RenderComponent::s_name));

					if(jsonEntity.contains(LightComponent::s_name) && !jsonEntity[LightComponent::s_name].is_null())
						entity->addComponent(m_pEntityFactory->createComponent(LightComponent::s_name));

					entity->from_json(jsonEntity);

					m_entities.push_back(entity);
				}
			}
		}
	}
}