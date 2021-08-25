#include "sgspch.h"
#include "SceneGraph.h"

namespace Sogas
{
	SceneGraph::SceneGraph()
	{
		m_root.reset(new RootNode());
	}

	SceneGraph::~SceneGraph()
	{
	}

	void SceneGraph::onRender()
	{
		if(m_root && m_currentCamera)
		{
			m_root->render(this);
			m_root->renderChildren(this);
			m_root->postRender(this);
		}
	}

	void SceneGraph::onUpdate()
	{
		// TODO: This will be implemented when animations are added to the engine
	}

	std::shared_ptr<ISceneNode> SceneGraph::findNode(NodeId id)
	{
		SceneNodeMap::iterator it = m_nodeMap.find(id);
		if (it == m_nodeMap.end())
			return std::shared_ptr<ISceneNode>();

		return (*it).second;
	}

	bool SceneGraph::addChild(NodeId id, std::shared_ptr<ISceneNode> child)
	{
		if(id >= (u32)MainRenderPass::LAST)
		{
			m_nodeMap[id] = child;
		}

		m_root->addChild(child);

		return true;
	}

	bool SceneGraph::removeChild(NodeId id)
	{
		if(id >= (u32)MainRenderPass::LAST)
			return false;

		std::shared_ptr<ISceneNode> child = findNode(id);

		m_nodeMap.erase(id);
		m_root->removeChild(id);

		return true;
	}
}