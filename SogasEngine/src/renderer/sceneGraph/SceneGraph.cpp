#include "sgspch.h"
#include "SceneGraph.h"
#include "scene/scene.h"
#include "scene/components/lightComponent.h"
#include "scene/components/entityComponent.h"
#include "scene/components/renderComponent.h"
#include "scene/components/transformComponent.h"
#include "scene/components/cameraComponent.h"

namespace Sogas
{
	SceneGraph::SceneGraph()
	{
		m_root.reset(new RootNode());
	}

	SceneGraph::~SceneGraph()
	{
	}

	// Call it the first time the scene graph is generated
	// The root node is supposed to be cleaned
	void SceneGraph::buildFromScene(std::weak_ptr<Scene> pScene)
	{
		std::vector<StrongEntityPtr>::iterator begin = pScene.lock()->m_rootEntities.begin();
		std::vector<StrongEntityPtr>::iterator end = pScene.lock()->m_rootEntities.end();

		for(std::vector<StrongEntityPtr>::iterator it = begin; it != end; it++)
		{
			// TODO: createTreeFromEntity, not only the current node
			std::shared_ptr<SceneNode> node = createNodeFromEntity((*it));
			m_root->addChild(node);
		}
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
	std::shared_ptr<SceneNode> SceneGraph::createNodeFromEntity(StrongEntityPtr entity)
	{
		// TODO: creater a wrapper or helper function for the ID that automatically increments it
		static u32 nodeId = (u32)MainRenderPass::LAST;

		if(entity->has<LightComponent>() && entity->has<RenderComponent>())
		{
			glm::mat4 transform = entity->getComponent<TransformComponent>().lock()->getLocalTransform();
			std::weak_ptr<Light> light = entity->getComponent<LightComponent>().lock()->getLight();

			std::shared_ptr<LightNode> lightNode = std::make_shared<LightNode>(LightNode(nodeId, transform, light));
			nodeId++;

			std::weak_ptr<Material> material = entity->getComponent<RenderComponent>().lock()->getMaterial();
			std::shared_ptr<MaterialNode> materialNode = std::make_shared<MaterialNode>(MaterialNode(nodeId, transform, material));
			nodeId++;

			std::weak_ptr<Mesh> mesh = entity->getComponent<RenderComponent>().lock()->getMesh();
			std::shared_ptr<GeometryNode> geometryNode = std::make_shared<GeometryNode>(GeometryNode(nodeId, transform, mesh));
			nodeId++;

			materialNode->addChild(geometryNode);
			lightNode->addChild(materialNode);

			return lightNode;
		}
		else if(entity->has<LightComponent>())
		{
			glm::mat4 transform = entity->getComponent<TransformComponent>().lock()->getLocalTransform();
			std::weak_ptr<Light> light = entity->getComponent<LightComponent>().lock()->getLight();

			std::shared_ptr<LightNode> lightNode = std::make_shared<LightNode>(LightNode(nodeId, transform, light));
			nodeId++;

			return lightNode;
		}
		else
		{
			glm::mat4 transform = entity->getComponent<TransformComponent>().lock()->getLocalTransform();
			std::shared_ptr<EmptyNode> emptyNode = std::make_shared<EmptyNode>(EmptyNode(nodeId, transform));
			nodeId++;

			return emptyNode;
		}			
	}
}