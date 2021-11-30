#include "sgspch.h"
#include "SceneNodes.h"

#include "core/camera.h"
#include "renderer/renderer.h"
#include "renderer/resources/light.h"
#include "renderer/resources/material.h"
#include "renderer/resources/mesh.h"
#include "renderer/resources/texture.h"

namespace Sogas
{
	SceneNode::SceneNode(const NodeId nodeId, const std::string name, const EntityId entityId)
	{
		m_properties.nodeId		= nodeId;
		m_properties.entityId	= entityId;
		m_properties.radius		= 0.0f;
		m_properties.transform  = glm::mat4(1); // TODO Should it be initialized to identity?
		m_properties.name		= name;
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
		return true;
	}

	void SceneNode::render(SceneGraph* pScene)
	{
		// TODO: return some kind of check
	}

	void SceneNode::renderChildren(SceneGraph* pScene)
	{
		for(SceneNodeList::iterator it = m_children.begin(); it != m_children.end(); it++)
		{
			(*it)->preRender(pScene); // TODO: add check to see if preRender went well
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

	void SceneNode::updateNode(void* data)
	{
		SGSDEBUG("SceneNode class should not call updateNode.");
		return;
	}

	RootNode::RootNode(const NodeId nodeId, const std::string name)
		: SceneNode(nodeId, name)
	{
		m_properties.setType(SceneNodeType::ROOT);

		m_children.reserve((size_t)MainRenderPass::LAST);

		std::shared_ptr<SceneNode> opaqueGroup(new SceneNode(
			(u32)MainRenderPass::OPAQUE, "OpaqueGroupNode"));
		m_children.push_back(opaqueGroup);

		std::shared_ptr<SceneNode> transparentGroup(new SceneNode(
			(u32)MainRenderPass::TRANSPARENT, "TransparentGroupNode"));
		m_children.push_back(transparentGroup);

		std::shared_ptr<SceneNode> environmentGroup(new SceneNode(
			(u32)MainRenderPass::ENVIRONMENT, "EnvironmentGroupNode"));
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
		m_children[(u32)MainRenderPass::OPAQUE - (u32)MainRenderPass::_0]->renderChildren(pScene);
	}

	void RootNode::resetNode()
	{
		// TODO: clean up the root node
	}

	CameraNode::CameraNode(const NodeId nodeId, const std::string name, const EntityId entityId)
		: SceneNode(nodeId, name, entityId)
	{
		m_pCamera = nullptr;
		m_properties.setType(SceneNodeType::CAMERA);
	}

	CameraNode::CameraNode(const NodeId nodeId, const std::string name, std::weak_ptr<Camera> camera, const EntityId entityId)
		: SceneNode(nodeId, name, entityId), m_pCamera(camera.lock().get())
	{
		m_properties.setType(SceneNodeType::CAMERA);
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

	void CameraNode::updateNode(void* data)
	{
		Camera* newCamera = reinterpret_cast<Camera*>(data);
		// TODO
	}

	void CameraNode::lookAt(const glm::vec3 position, const glm::vec3 center)
	{
		m_pCamera->lookat(position, center);
	}

	glm::mat4 CameraNode::getView()
	{
		return m_pCamera->getView();
	}

	glm::mat4 CameraNode::getProjection()
	{
		return m_pCamera->getProjection();
	}

	glm::mat4 CameraNode::getViewProjection()
	{
		return m_pCamera->getViewProjection();
	}

	inline void CameraNode::setPosition(glm::vec3 pos)
	{
		m_pCamera->setPosition(pos);
	}

	LightNode::LightNode(const NodeId nodeId, std::string name, const EntityId entityId)
		: SceneNode(nodeId, name, entityId)
	{
		m_pLight = nullptr;
		m_properties.setType(SceneNodeType::LIGHT);
	}

	LightNode::LightNode(const NodeId nodeId, const std::string name, std::weak_ptr<Light> light, const EntityId entityId)
		: SceneNode(nodeId, name, entityId), m_pLight(light.lock().get())
	{
		m_properties.setType(SceneNodeType::LIGHT);
	}

	void LightNode::updateNode(void* data)
	{
		m_pLight = static_cast<Light*>(data);
	}

	MaterialNode::MaterialNode(const NodeId nodeId, std::string name, const EntityId entityId)
		: SceneNode(nodeId, name, entityId)
	{
		m_properties.setType(SceneNodeType::MATERIAL);
		m_pMaterial = nullptr;
	}

	MaterialNode::MaterialNode(const NodeId nodeId, const std::string name, std::weak_ptr<Material> material, const EntityId entityId)
		: SceneNode(nodeId, name, entityId), m_pMaterial(material.lock().get())
	{
		m_properties.setType(SceneNodeType::MATERIAL);
	}

	void MaterialNode::preRender(SceneGraph* pScene)
	{
		// set material stuff (uniforms and shaders)
		// call renderer functions to set material

		// [TODO]: Should this be setting the transform? Or should each geometry be awared of its position?
		if(m_pMaterial)
			Renderer::get()->submit(m_pMaterial, getNodeProperties()->getNodeId());
	}

	void MaterialNode::updateNode(void* data)
	{
		m_pMaterial = static_cast<Material*>(data);
	}

	GeometryNode::GeometryNode(const NodeId nodeId, std::string name, const EntityId entityId)
		: m_primitive(Primitive::TRIANGLES), SceneNode(nodeId, name, entityId)
	{
		m_properties.setType(SceneNodeType::GEOMETRY);
		m_pMesh = nullptr;
	}

	GeometryNode::GeometryNode(const NodeId nodeId, const std::string name, std::weak_ptr<Mesh> mesh, Primitive primitive, const EntityId entityId)
		: SceneNode(nodeId, name, entityId), m_pMesh(mesh.lock().get()), m_primitive(primitive)
	{
		m_properties.setType(SceneNodeType::GEOMETRY);
	}

	void GeometryNode::render(SceneGraph* pScene)
	{
		// should use the material that is set as render state
		// by a previous material node

		// TODO: render indexed
		// call renderer::submit with the mesh, the material used will be the set by a previous material node
		if(m_pMesh)
			Renderer::get()->submit(m_pMesh, m_primitive);
	}

	void GeometryNode::updateNode(void* data)
	{
		m_pMesh = static_cast<Mesh*>(data);
	}

	EnvironmentNode::EnvironmentNode(const NodeId nodeId, std::string name, const EntityId entityId)
		: SceneNode(nodeId, name, entityId)
	{
		m_pEnvironment = nullptr;
		m_properties.setType(SceneNodeType::ENVIRONMENT);
	}

	EnvironmentNode::EnvironmentNode(const NodeId nodeId, const std::string name, std::weak_ptr<Environment> environment, const EntityId entityId)
		: SceneNode(nodeId, name, entityId), m_pEnvironment(environment.lock().get())
	{
		m_properties.setType(SceneNodeType::ENVIRONMENT);
	}

	void EnvironmentNode::preRender(SceneGraph* pScene)
	{
		// TODO: set transform to camera position
	}

	void EnvironmentNode::render(SceneGraph* pScene)
	{
		// TODO: render the sky same way as the EnvironmentComponent
	}

	void EnvironmentNode::updateNode(void* data)
	{
	}

	TransformNode::TransformNode(NodeId nodeId, std::string name, const EntityId entityId)
		: SceneNode(nodeId, name, entityId)
	{
		m_transform = glm::mat4(1);
		m_properties.setType(SceneNodeType::TRANSFORM);
	}

	void TransformNode::preRender(SceneGraph* pScene)
	{
		Renderer::get()->submit(m_properties.getTransform());
	}

	void TransformNode::updateNode(void* data)
	{
		glm::mat4* mat = static_cast<glm::mat4*>(data);
		m_properties.setTransform(*mat);
	}
}