#pragma once

#include "renderer/renderer.h"
#include "SceneNodes.h"

namespace Sogas
{
	class Scene;

	typedef std::map<NodeId, std::shared_ptr<ISceneNode>> SceneNodeMap;

	class SceneGraph
	{
	private:
		std::shared_ptr<SceneNode> m_root;
		std::shared_ptr<CameraNode> m_currentCamera;
		SceneNodeMap m_nodeMap;

	public:
		SceneGraph();
		~SceneGraph();

		void buildFromScene(std::weak_ptr<Scene> pScene);

		void onRender();
		void onUpdate();

		std::shared_ptr<ISceneNode> findNode(NodeId id);
		bool addChild(NodeId id, std::shared_ptr<ISceneNode> child);
		bool removeChild(NodeId id);

		void setCamera(std::shared_ptr<CameraNode> camera) { m_currentCamera = camera; }
		std::shared_ptr<CameraNode> getCamera() const { return m_currentCamera; }

	private:
		std::shared_ptr<SceneNode> createNodeFromEntity(StrongEntityPtr entity);
	};
}