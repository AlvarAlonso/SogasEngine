
#include "sgspch.h"
#include "scene.h"
#include "renderer/sceneGraph/SceneGraph.h"
#include "components/transformComponent.h"
#include "components/renderComponent.h"
#include "components/lightComponent.h"
#include "components/cameraComponent.h"
#include "components/entityScriptComponent.h"

namespace Sogas
{
	Scene::Scene()
	{
		m_pEntityFactory.reset();
		m_pEntityFactory = std::make_unique<EntityFactory>();
		m_pEnvironment = std::make_shared<Environment>();
		m_sceneGraph = std::make_shared<SceneGraph>();
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
	StrongEntityPtr Scene::createEntity(const std::string& name, const EntityId parentId)
	{
		StrongEntityPtr entity = m_pEntityFactory->createEntity(name.c_str());
		entity->setScene(shared_from_this());
		m_entities.push_back(entity);	// Add the entity to the vector that holds all entities.

		// Also check if it has a parent. If it has not, add it to the entitiesHierarchy that holds all root entities.
		// If it has a parent ID, add as a child to that parent.
		if(parentId == 0)
			m_rootEntities.push_back(entity);
		else {
			const auto& parent = findEntityById(parentId);
			parent->addChild(entity);
		}
		return entity;
	}

	/*
	* @brief Destroy the entity from the scene
	*/
	void Scene::destroyEntity(EntityId entityId)
	{
		// TODO: Optimize insertion/deletion of entities
		i32 index = 0;
		for(auto& currentEntity : m_entities)
		{
			if (currentEntity->getId() == entityId)
			{
				// If the entity is a root, erase from the root vector.
				if (!currentEntity->hasParent()) {
					i32 i = 0;
					for (auto& ent : m_rootEntities)
					{
						if (ent->getId() == entityId) {
							m_rootEntities.erase(m_rootEntities.begin() + i);
							break;
						}
						i++;
					}
				}
				else {
					currentEntity->getParent()->removeChild(entityId);
				}

				// If it has childs, call their destruction recursively.
				if (currentEntity->hasChild()) {
					auto& childs = currentEntity->getChildList();
					for (auto child : childs) {
						destroyEntity(child->getId());
					}
				}
				currentEntity->destroy();
				currentEntity = nullptr;
				m_entities.erase(m_entities.begin() + index);

				break; // break loop after finding the entity to be deleted
			}
			index++;
		}

		// TODO: some way to destroy an entity deleting all the references to it automatically
	}

	/*
	* @brief Remove the entity from the m_entities vector only. This is used for making entity the child of
	* an already existing Entity.
	*/
	void Scene::removeEntity(EntityId entityId)
	{
		i32 index = 0;
		for (auto& currentEntity : m_rootEntities)
		{
			if (currentEntity->getId() == entityId)
			{
				m_rootEntities.erase(m_rootEntities.begin() + index);
				break; // break loop after finding the entity to be deleted
			}

			index++;
		}
	}

	// TODO: scene onUpdate should update and submit commands to the renderer, thus the renderer needs to have access to the shader
	// and textures to bind them when updating/rendering
	void Scene::onUpdate(f32 dt)
	{
		for(auto& entity : m_entities)
		{
			auto scriptComponent = entity->getComponent<EntityScriptComponent>();

			if (scriptComponent.lock())
			{
				scriptComponent.lock()->update(dt);
			}
		}
		//for (auto& entity : m_entities)
		//{
		//	auto tComponent = entity->getComponent<TransformComponent>(TransformComponent::s_name);
		//}

	}

	void Scene::onRender(std::shared_ptr<Camera> pCamera)
	{
		std::shared_ptr<Scene> pScene = shared_from_this();

		// render scene
		m_sceneGraph->onRender(pScene, pCamera);
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

	bool Scene::buildSceneGraph()
	{
		return m_sceneGraph->buildFromScene(shared_from_this());
	}

	void Scene::setCamera(std::shared_ptr<Camera> pCamera)
	{
		m_pMainCamera = pCamera;
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
					entity->setScene(shared_from_this());

					if(jsonEntity.contains(RenderComponent::s_name) && !jsonEntity[RenderComponent::s_name].is_null())
						addComponent<RenderComponent>(entity);

					if (jsonEntity.contains(LightComponent::s_name) && !jsonEntity[LightComponent::s_name].is_null())
						addComponent<LightComponent>(entity);

					entity->from_json(jsonEntity);

					m_entities.push_back(entity);
					m_rootEntities.push_back(entity);
				}
			}
		}
	}
}