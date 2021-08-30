#include "sgspch.h"
#include "SceneNodes.h"

#include "core/camera.h"
#include "renderer/resources/light.h"
#include "renderer/resources/material.h"
#include "renderer/resources/mesh.h"
#include "renderer/resources/texture.h"

namespace Sogas
{
	SceneNode::SceneNode(const NodeId nodeId, const glm::mat4 transform, const std::string name)
	{
		m_properties.nodeId = nodeId;
		m_properties.radius = 0.0f;
		m_properties.transform = transform;
		m_properties.name = name;
		m_properties.renderPass = MainRenderPass::OPAQUE;
	}

	SceneNode::~SceneNode()
	{
	}

	void SceneNode::setTransform(glm::mat4 transform)
	{
		m_properties.transform = transform;
	}

	void SceneNode::preRender(SceneGraph* pScene)
	{
		// TODO: accumulate transform matrix to current transform matrix
		// or each node stores the actual matrix too
	}

	bool SceneNode::isVisible(SceneGraph* pScene) const
	{
		// TODO: check if current node is inside the camera's frustum
		return false;
	}

	void SceneNode::render(SceneGraph* pScene)
	{
		// TODO: return some kind of check
	}

	void SceneNode::renderChildren(SceneGraph* pScene)
	{
		for(SceneNodeList::iterator it = m_children.begin(); it != m_children.end(); it++)
		{
			if((*it)->isVisible(pScene))
			{
				(*it)->render(pScene);
			}

			(*it)->renderChildren(pScene);
			(*it)->postRender(pScene);
		}
	}

	void SceneNode::postRender(SceneGraph* pScene)
	{
		// TODO: return some check
	}

	bool SceneNode::addChild(std::shared_ptr<ISceneNode> child)
	{
		m_children.push_back(child);
		// TODO: extend parent radius

		return true;
	}

	bool SceneNode::removeChild(NodeId id)
	{
		for(SceneNodeList::iterator it = m_children.begin(); it != m_children.end(); it++)
		{
			const SceneNodeProperties* pProperties = (*it)->getNodeProperties();
			if((pProperties->nodeId != 0 && id == pProperties->nodeId))
			{
				it = m_children.erase(it);
				return true;
			}
		}

		return false;
	}

	RootNode::RootNode(const NodeId nodeId, glm::mat4 transform, const std::string name)
		: SceneNode(nodeId, transform, name)
	{
		m_children.reserve((size_t)MainRenderPass::LAST);

		std::shared_ptr<SceneNode> opaqueGroup(new SceneNode(
			(u32)MainRenderPass::OPAQUE, glm::mat4(1), "OpaqueGroupNode"));
		m_children.push_back(opaqueGroup);

		std::shared_ptr<SceneNode> transparentGroup(new SceneNode(
			(u32)MainRenderPass::TRANSPARENT, glm::mat4(1), "TransparentGroupNode"));
		m_children.push_back(transparentGroup);

		std::shared_ptr<SceneNode> environmentGroup(new SceneNode(
			(u32)MainRenderPass::ENVIRONMENT, glm::mat4(1), "EnvironmentGroupNode"));
		m_children.push_back(environmentGroup);
	}

	bool RootNode::addChild(std::shared_ptr<ISceneNode> child)
	{
		MainRenderPass renderPass = child->getNodeProperties()->getMainRenderPass();

		if (renderPass < MainRenderPass::_0 || renderPass >= MainRenderPass::LAST)
			SGSERROR("The render pass does not exist!");

		return m_children[(size_t)renderPass - (size_t)MainRenderPass::_0]->addChild(child);
	}

	void RootNode::renderChildren(SceneGraph* pScene)
	{
	}

	void RootNode::resetNode()
	{

	}

	CameraNode::CameraNode(const NodeId nodeId, const glm::mat4 transform, const std::string name, std::weak_ptr<Camera> camera)
		: SceneNode(nodeId, transform, name), m_camera(camera)
	{
	}

	void CameraNode::render(SceneGraph* pScene)
	{
		// TODO: draw frustum and icon of the game camera
	}

	bool CameraNode::isVisible(SceneGraph* pScene) const
	{
		// TODO: return if the camera is active or not, even though not active cameras
		// could be drawn also in debug mode.
		return true;
	}

	void CameraNode::lookAt(const glm::vec3 position, const glm::vec3 center)
	{
		m_camera.lock()->lookat(position, center);
	}

	glm::mat4 CameraNode::getView()
	{
		return m_camera.lock()->getView();
	}

	glm::mat4 CameraNode::getProjection()
	{
		return m_camera.lock()->getProjection();
	}

	glm::mat4 CameraNode::getViewProjection()
	{
		return m_camera.lock()->getViewProjection();
	}

	inline void CameraNode::setPosition(glm::vec3 pos)
	{
		m_camera.lock()->setPosition(pos);
	}

	LightNode::LightNode(const NodeId nodeId, const glm::mat4 transform, const std::string name, std::weak_ptr<Light> light)
		: SceneNode(nodeId, transform, name), m_light(light)
	{
	}

	MaterialNode::MaterialNode(const NodeId nodeId, const glm::mat4 transform, const std::string name, std::weak_ptr<Material> material)
		: SceneNode(nodeId, transform, name), m_material(material)
	{
	}

	void MaterialNode::preRender(SceneGraph* pScene)
	{
		// TODO: set material stuff (uniforms and shaders)
	}

	GeometryNode::GeometryNode(const NodeId nodeId, const glm::mat4 transform, const std::string name, std::weak_ptr<Mesh> mesh)
		: SceneNode(nodeId, transform, name), m_mesh(mesh)
	{
	}

	void GeometryNode::render(SceneGraph* pScene)
	{
		// TODO: should use the material that is set as render state
		// by a previous material node

		// TODO: render indexed
	}

	EnvironmentNode::EnvironmentNode(const NodeId nodeId, const glm::mat4 transform, const std::string name, std::weak_ptr<Environment> environment)
		: SceneNode(nodeId, transform, name), m_environment(environment)
	{
	}

	void EnvironmentNode::preRender(SceneGraph* pScene)
	{
		// TODO: set transform to camera position
	}

	void EnvironmentNode::render(SceneGraph* pScene)
	{
		// TODO: render the sky same way as the EnvironmentComponent
	}
}