#pragma once

#include "renderer/renderer.h"
#include "SceneNodes.h"
#include "scene/entity.h"

namespace Sogas
{
	class Scene;

	// TODO: A map that maps ActorId and SceneNodeTypeChanged to the corresponding ISceneNode. A double map can be used.
	// The first, maps the ActorId to its map of SceneNodes. The second maps the name of the scene node changed to the 
	// corresponding ISceneNode.

	// This maps actors to its potential diferent nodes

	typedef std::map<SceneNodeType, std::shared_ptr<ISceneNode>> TypeNodeMap;
	typedef std::map<EntityId, TypeNodeMap> EntityToSceneNodesMap;

	typedef std::map<NodeId, std::shared_ptr<ISceneNode>> SceneNodeMap;

	class AlphaSceneNode
	{
	public:
		std::shared_ptr<SceneNode> m_node;
		std::weak_ptr<Material> m_material;
		glm::mat4 m_globalTransform;
		f32 m_distanceToCamera;

		bool const operator < (AlphaSceneNode const& other)
		{
			return m_distanceToCamera < other.m_distanceToCamera;
		}
	};

	class SceneGraph
	{
	private:
		std::shared_ptr<SceneNode> m_root;
		std::shared_ptr<CameraNode> m_currentCamera;
		SceneNodeMap m_nodeMap;
		EntityToSceneNodesMap m_entityNodesMap;
		std::list<AlphaSceneNode> m_alphaNodes;

	public:
		SceneGraph();
		~SceneGraph();

		bool buildFromScene(std::weak_ptr<Scene> pScene);

		template<typename T>
		void addNode(EntityId entityId, glm::mat4 transform, void* data)
		{
			NodeId nodeId = getNextNodeID();
			StrongEntityPtr entity = Scene::findEntityById(entityId);
			
			if(T::getStaticName() == "_empty")
			{
				std::shared_ptr<T> newNode = std::make_shared<T>(T(getNextNodeID(), transform, entity->getName()));
				return;
			}

			std::string nodeName = entity->getName().append(T::getStaticName());
			// generate new node for an existing entity represented by an empty node
			std::shared_ptr<T> newNode = std::make_shared<T>(T(getNextNodeID(), transform, nodeName));
			// populate the new node
			newNode->updateNode(data);
		}

		void onRender(std::shared_ptr<Scene>& pScene, std::shared_ptr<Camera>& pCamera);
		void onUpdate();

		std::shared_ptr<ISceneNode> findNode(NodeId id);
		bool addChild(NodeId id, std::shared_ptr<ISceneNode> child);
		bool removeChild(NodeId id);
		void addAlphaSceneNode(AlphaSceneNode node);
		void updateNode(EntityId entityId, SceneNodeType nodeType, void* data);

		void setCamera(std::shared_ptr<CameraNode> camera) { m_currentCamera = camera; }
		std::shared_ptr<CameraNode> getCamera() const { return m_currentCamera; }

	private:

		void renderAlphaPass();

		NodeId getNextNodeID();

		std::shared_ptr<SceneNode> createTreeFromEntity(StrongEntityPtr entity);
		std::shared_ptr<SceneNode> createNodesFromEntity(StrongEntityPtr entity);
	};
}