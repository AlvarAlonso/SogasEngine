#include "sgspch.h"
#include "SceneNodes.h"

#include "core/camera.h"
#include "renderer/resources/light.h"
#include "renderer/resources/material.h"
#include "renderer/resources/mesh.h"
#include "renderer/resources/texture.h"

namespace Sogas
{
	SceneNode::SceneNode(const NodeId nodeId, const glm::mat4 transform)
	{
		m_properties.nodeId = nodeId;
		m_properties.radius = 0.0f;
		m_properties.transform = transform;
	}

	SceneNode::~SceneNode()
	{
	}

	void SceneNode::setTransform(glm::mat4 transform)
	{
	}

	void SceneNode::preRender(SceneGraph* pScene)
	{
	}

	bool SceneNode::isVisible(SceneGraph* pScene) const
	{
		return false;
	}

	void SceneNode::render(SceneGraph* pScene)
	{
	}

	void SceneNode::renderChildren(SceneGraph* pScene)
	{
	}

	void SceneNode::postRender(SceneGraph* pScene)
	{
	}

	bool SceneNode::addChild(std::shared_ptr<ISceneNode> child)
	{
		return false;
	}

	void SceneNode::removeChild(NodeId id)
	{
	}

	CameraNode::CameraNode(const NodeId nodeId, const glm::mat4 transform, std::weak_ptr<Camera> camera)
		: SceneNode(nodeId, transform), m_camera(camera)
	{
	}

	void CameraNode::render(SceneGraph* pScene)
	{
	}

	bool CameraNode::isVisible(SceneGraph* pScene) const
	{
		return false;
	}

	void CameraNode::lookAt(const glm::vec3 position, const glm::vec3 center)
	{
	}

	glm::mat4 CameraNode::getView()
	{
		return glm::mat4();
	}

	glm::mat4 CameraNode::getProjection()
	{
		return glm::mat4();
	}

	glm::mat4 CameraNode::getViewProjection()
	{
		return glm::mat4();
	}

	inline void CameraNode::setPosition(glm::vec3 pos)
	{
	}

	RootNode::RootNode(const NodeId nodeId, glm::mat4 transform)
		: SceneNode(nodeId, transform)
	{
	}

	bool RootNode::addChild(std::shared_ptr<ISceneNode> child)
	{
		return false;
	}

	void RootNode::renderChildren(SceneGraph* pScene)
	{
	}

	LightNode::LightNode(const NodeId nodeId, const glm::mat4 transform, std::weak_ptr<Light> light)
		: SceneNode(nodeId, transform), m_light(light)
	{
	}

	MaterialNode::MaterialNode(const NodeId nodeId, const glm::mat4 transform, std::weak_ptr<Material> material)
		: SceneNode(nodeId, transform), m_material(material)
	{
	}

	void MaterialNode::preRender(SceneGraph* pScene)
	{
	}

	GeometryNode::GeometryNode(const NodeId nodeId, const glm::mat4 transform, std::weak_ptr<Mesh> mesh)
		: SceneNode(nodeId, transform), m_mesh(mesh)
	{
	}

	void GeometryNode::render(SceneGraph* pScene)
	{
	}

	EnvironmentNode::EnvironmentNode(const NodeId nodeId, const glm::mat4 transform, std::weak_ptr<Environment> environment)
		: SceneNode(nodeId, transform), m_environment(environment)
	{
	}

	void EnvironmentNode::preRender(SceneGraph* pScene)
	{
	}

	void EnvironmentNode::render(SceneGraph* pScene)
	{
	}
}