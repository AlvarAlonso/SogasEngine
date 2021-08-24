#include "sgspch.h"
#include "SceneNodes.h"

namespace Sogas
{
	SceneNode::SceneNode(NodeId nodeId, const glm::mat4 transform)
	{
	}

	SceneNode::~SceneNode()
	{
	}

	void SceneNode::setTransform(glm::mat4 transform)
	{
	}

	void SceneNode::preRender(Scene* pScene)
	{
	}

	void SceneNode::renderChildren(Scene* pScene)
	{
	}

	void SceneNode::postRender(Scene* pScene)
	{
	}

	bool SceneNode::addChild(std::shared_ptr<ISceneNode> child)
	{
		return false;
	}

	void SceneNode::removeChild(NodeId id)
	{
	}
}
