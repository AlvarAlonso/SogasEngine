#pragma once

#include "renderer/renderer.h"
#include "SceneGraph.h"

namespace Sogas
{
	class ISceneNode;
	class CameraNode;
	class EnvironmentNode;
	class LightNode;
	class SceneNode;

	typedef std::map<NodeId, std::shared_ptr<ISceneNode>> SceneNodeMap;

	class SceneGraph
	{
	private:
		std::shared_ptr<SceneNode> m_root;
		std::shared_ptr<CameraNode> m_currentCamera;

	public:
		SceneGraph();
		~SceneGraph();

		void onRender();
		void onUpdate();

		std::shared_ptr<ISceneNode> findNode(NodeId id);
		bool addChild(NodeId id, std::shared_ptr<ISceneNode> child);
		bool removeChild(NodeId id);

		void setCamera(std::shared_ptr<CameraNode> camera) { m_currentCamera = camera; }
		std::shared_ptr<CameraNode> getCamera() const { return m_currentCamera; }
	};
}

