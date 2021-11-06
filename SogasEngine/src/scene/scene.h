#pragma once

#include "defines.h"

#include "entity.h"
#include "entityFactory.h"
#include "environment.h"
#include "renderer/sceneGraph/SceneGraph.h"
#include <json/single_include/nlohmann/json.hpp>

namespace Sogas 
{
	using json = nlohmann::json;

	class Environment;
	class Camera;

	class SGS Scene : public std::enable_shared_from_this<Scene>
	{
	public:
		bool m_renderEnvironment{ true };

	private:
		std::vector<StrongEntityPtr>	m_entities;
		std::vector<StrongEntityPtr>	m_rootEntities;
		std::unique_ptr<EntityFactory>	m_pEntityFactory;
		std::shared_ptr<Environment>	m_pEnvironment;
		StrongEntityPtr					m_selectedEntity;

		std::shared_ptr<SceneGraph>		m_sceneGraph;

		std::shared_ptr<Camera>			m_pMainCamera; // [TEMPORAL]: SceneGraph should create cameraNode from a CameraComponent

		friend class SceneGraph;

	public:
		Scene();
		Scene(const char* filename);
		~Scene();

		StrongEntityPtr createEntity(const std::string& name = std::string(), const EntityId parentId = 0);
		void			destroyEntity(EntityId entityId);
		void			removeEntity(EntityId entityId);
		void			onUpdate(f32 dt);
		void			onRender(std::shared_ptr<Camera> pCamera);

		const std::vector<StrongEntityPtr>& getEntities() { return m_entities; };
		const std::vector<StrongEntityPtr>& getRootEntities() { return m_rootEntities; }
		std::weak_ptr<Environment>			getEnvironment() { return m_pEnvironment; }
		void								setSelectedEntity(StrongEntityPtr entity) { m_selectedEntity = entity; }
		StrongEntityPtr						getSelectedEntity(void) { return m_selectedEntity ? m_selectedEntity : nullptr; }

		StrongEntityPtr findEntityById(EntityId entityId);

		bool buildSceneGraph();

		/*
		template<typename T>
		void addSceneGraphNode(StrongEntityPtr entity, glm::mat4 transform, void* data)
		{
			m_sceneGraph->addNode<T>(entity, transform, data);
		}
		*/

		void updateSceneGraphNode(EntityId entityId, SceneNodeType nodeType, void* data);
		void setCamera(std::shared_ptr<Camera> pCamera); // TODO: [Temporal]: This should change when the camera is created from a CameraComponent

		void to_json(json& j);
		void from_json(const json& j);

		/*
		* Adds component to an entity
		* @param StrongEntityPtr
		* @return void
		*/
		template <class T>
		void addComponent(StrongEntityPtr entity)
		{
			// TODO: maybe put the emptyNode creation also here, instead of handling it during entity creation

			//if (!T:s_name)
			//{
			//	SGSWARN("No valid component name given.");
			//	return;
			//}

			// TODO: name from type may not be the same as stored in static s_name
			// Should find a more robust way to create a specific component type
			StrongEntityComponentPtr pComponent = m_pEntityFactory->createComponent(T::s_name);
			entity->addComponent(pComponent);
			//pComponent->setOwner(entity);

			// add node to render scene graph 
			// TODO: refactor this which is currently hardcoded and avoid runtime type checking
			std::string componentName = T::s_name;
			if(componentName == "RenderComponent")
			{
				m_sceneGraph->addNode<MaterialNode>(entity, glm::mat4(1), (void*)&dynamic_cast<RenderComponent*>(pComponent.get())->getMesh());
				m_sceneGraph->addNode<GeometryNode>(entity, glm::mat4(1), (void*)&dynamic_cast<RenderComponent*>(pComponent.get())->getMaterial());
			}
			else if (componentName == "LightComponent")
			{
				//m_sceneGraph->addNode<LightNode>(entity, glm::mat4(1), (LightComponent)pComponent->getLight());
			}
		}

		/*
		* Get all entities with a given component
		* @param void
		* @return std::vector<StrongEntityPtr>
		*/
		template <class T>
		std::vector<StrongEntityPtr> getByComponent()
		{
			std::vector<StrongEntityPtr> returnVector{};

			for (auto& entity : m_entities)
			{
				if (entity->has<T>())
				{
					returnVector.push_back(entity);
				}
			}
			return returnVector;
		}
	};
}