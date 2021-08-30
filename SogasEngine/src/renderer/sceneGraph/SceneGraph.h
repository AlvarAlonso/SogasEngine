#pragma once

#include "renderer/renderer.h"
#include "SceneNodes.h"

namespace Sogas
{
	class Scene;

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
		std::list<AlphaSceneNode> m_alphaNodes;

	public:
		SceneGraph();
		~SceneGraph();

		bool buildFromScene(std::weak_ptr<Scene> pScene);

		void onRender();
		void onUpdate();

		std::shared_ptr<ISceneNode> findNode(NodeId id);
		bool addChild(NodeId id, std::shared_ptr<ISceneNode> child);
		bool removeChild(NodeId id);
		void addAlphaSceneNode(AlphaSceneNode node);

		void setCamera(std::shared_ptr<CameraNode> camera) { m_currentCamera = camera; }
		std::shared_ptr<CameraNode> getCamera() const { return m_currentCamera; }

	private:

		void renderAlphaPass();

		std::shared_ptr<SceneNode> createTreeFromEntity(StrongEntityPtr entity);
		std::shared_ptr<SceneNode> createNodesFromEntity(StrongEntityPtr entity);
	};
}