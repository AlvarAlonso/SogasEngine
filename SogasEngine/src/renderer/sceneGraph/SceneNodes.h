#pragma once

#include <memory>
#include <glm/glm/mat4x4.hpp>
#include "scene/types.h"

namespace Sogas
{
	class Scene;

	struct SceneNodeProperties
	{
	private:
		NodeId nodeId;
		glm::mat4 transform;
		float radius;

	public:
		const NodeId& getNodeId() const { return nodeId; }
		glm::mat4 const& getTransform() const { return transform; }
		float getRadius() const { return radius; }
	};

	class ISceneNode
	{
	public:
		virtual const SceneNodeProperties* const getNodeProperties() const = 0;

		virtual void setTransform(glm::mat4 transform) = 0;
		virtual void preRender(Scene* pScene) = 0;
		virtual void renderChildren(Scene* pScene) = 0;
		virtual void postRender(Scene* pScene) = 0;
	
		virtual bool addChild(std::shared_ptr<ISceneNode> child) = 0;
		virtual void removeChild(NodeId id) = 0;

		virtual ~ISceneNode() {};
	};


	class SceneNode : public ISceneNode
	{
	private:
		SceneNodeList m_children;
		std::weak_ptr<SceneNode> m_parent;
		SceneNodeProperties m_properties;
	public:
		SceneNode(NodeId nodeId, const glm::mat4 transform);
		virtual ~SceneNode();
		virtual const SceneNodeProperties* const getNodeProperties() const override { return &m_properties; };
		virtual void setTransform(glm::mat4 transform);
		
		virtual void preRender(Scene* pScene);
		virtual void renderChildren(Scene* pScene);
		virtual void postRender(Scene* pScene);

		virtual bool addChild(std::shared_ptr<ISceneNode> child);
		virtual void removeChild(NodeId id);
	};
}
