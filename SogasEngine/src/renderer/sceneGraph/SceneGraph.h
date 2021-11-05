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
		std::shared_ptr<SceneNode> m_pNode;
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
		void addNode(StrongEntityPtr entity, glm::mat4 transform, void* data)
		{
			NodeId nodeId = getNextNodeID();
			
			if(T::getStaticName() == "_empty")
			{
				// TODO: WTF is that
				std::shared_ptr<T> newNode = std::make_shared<T>(T(getNextNodeID(), transform, entity->getName()));
				return;
			}

			std::string nodeName = entity->getName().append(T::getStaticName());
			// generate new node for an existing entity represented by an empty node
			std::shared_ptr<T> newNode = std::make_shared<T>(T(getNextNodeID(), transform, nodeName));
			// populate the new node
			newNode->updateNode(data);

			// TODO: place node in the scene graph. If it has a parent, it must be placed as a child of said parent. If not,
			// it is placed as a child of one of the root node groups.
			placeNode(newNode, entity->getId(), entity->getParent()->getId());
		}

		void onRender(std::shared_ptr<Scene>& pScene, std::shared_ptr<Camera>& pCamera);
		void onUpdate();

		std::shared_ptr<ISceneNode> findNode(NodeId id);
		std::shared_ptr<ISceneNode> findNode(EntityId entityId, SceneNodeType nodeType);
		std::shared_ptr<ISceneNode> findLastEntityNode(EntityId entityId, SceneNodeType nodeType = SceneNodeType::LAST);

		bool addChild(NodeId id, std::shared_ptr<ISceneNode> child);
		bool removeChild(NodeId id);
		void addAlphaSceneNode(AlphaSceneNode node);
		void updateNode(EntityId entityId, SceneNodeType nodeType, void* data);

		void setCamera(std::shared_ptr<CameraNode> camera) { m_currentCamera = camera; }
		std::shared_ptr<CameraNode> getCamera() const { return m_currentCamera; }

	private:

		void renderAlphaPass();

		NodeId getNextNodeID();

		bool placeNode(std::shared_ptr<SceneNode> node, EntityId entityId, EntityId parentId = 0);

		std::shared_ptr<SceneNode> createTreeFromEntity(StrongEntityPtr entity);
		std::shared_ptr<SceneNode> createNodesFromEntity(StrongEntityPtr entity);
	};
}