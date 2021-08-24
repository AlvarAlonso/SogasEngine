#include "sgspch.h"
#include "SceneGraph.h"

namespace Sogas
{
	SceneGraph::SceneGraph()
	{
	}

	SceneGraph::~SceneGraph()
	{
	}

	void SceneGraph::onRender()
	{
	}

	void SceneGraph::onUpdate()
	{
	}

	std::shared_ptr<ISceneNode> SceneGraph::findNode(NodeId id)
	{
		return std::shared_ptr<ISceneNode>();
	}

	bool SceneGraph::addChild(NodeId id, std::shared_ptr<ISceneNode> child)
	{
		return false;
	}

	bool SceneGraph::removeChild(NodeId id)
	{
		return false;
	}

}