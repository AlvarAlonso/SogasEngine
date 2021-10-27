#include "sgspch.h"
#include "SceneGraph.h"
#include "scene/scene.h"
#include "renderer/framebuffer.h"
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
	bool SceneGraph::buildFromScene(std::weak_ptr<Scene> pScene)
	{
		bool result;

		std::vector<StrongEntityPtr>::iterator begin = pScene.lock()->m_rootEntities.begin();
		std::vector<StrongEntityPtr>::iterator end = pScene.lock()->m_rootEntities.end();

		for(std::vector<StrongEntityPtr>::iterator it = begin; it != end; it++)
		{
			std::shared_ptr<SceneNode> node = createTreeFromEntity((*it));
			result = m_root->addChild(node);

			if (result == false)
				return false; // TODO: clean all the nodes created
		}

		m_currentCamera = std::make_shared<CameraNode>(getNextNodeID(), glm::mat4(1), "main camera", pScene.lock()->m_pMainCamera);

		return true;
	}

	void SceneGraph::onRender(std::shared_ptr<Scene> &pScene, std::shared_ptr<Camera> &pCamera)
	{
		Renderer::get()->beginScene(pScene, pCamera);
		Renderer::get()->getDefaultFramebuffer()->clearAttachment(1, -1);

		// TODO This should be after all renderables have been drawn, the problem is that the objects highlighted outline would not be seen.
		
		if (pScene->m_renderEnvironment) {
			Renderer::get()->renderEnvironment(pScene->getEnvironment());
		}
		
		
		if(m_root && m_currentCamera)
		{
			m_root->render(this);
			m_root->renderChildren(this);
			m_root->postRender(this);
		}
		
		Renderer::get()->endScene();
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

	void SceneGraph::addAlphaSceneNode(AlphaSceneNode node)
	{
		m_alphaNodes.push_back(node);
	}

	void SceneGraph::renderAlphaPass()
	{
		// TODO: render alpha pass
	}

	NodeId SceneGraph::getNextNodeID()
	{
		static u32 nodeId = (u32)MainRenderPass::LAST;

		u32 nextID = nodeId;
		nodeId++;

		return nextID;
	}

	std::shared_ptr<SceneNode> SceneGraph::createTreeFromEntity(StrongEntityPtr entity)
	{
		// create the nodes of the entity
		std::shared_ptr<SceneNode> entityTree = createNodesFromEntity(entity);

		// find the pointer to the last node
		
		std::shared_ptr<ISceneNode> lastNode = entityTree;
		while(lastNode->hasChildren())
		{
			//std::shared_ptr<ISceneNode> aux = lastNode->getChildren()[0];
			lastNode = lastNode->getChildren()[0];
		}
		
		// attach the nodes of its children
		if(entity->hasChild())
		{
			std::vector<StrongEntityPtr> children = entity->getChildList();
			std::vector<StrongEntityPtr>::iterator begin = children.begin();
			std::vector<StrongEntityPtr>::iterator end = children.end();

			for(std::vector<StrongEntityPtr>::iterator it = begin; it != end; it++)
			{
				std::shared_ptr<SceneNode> childEntityTree = createTreeFromEntity((*it));
				lastNode->addChild(childEntityTree);
			}
		}

		return entityTree;
	}

	std::shared_ptr<SceneNode> SceneGraph::createNodesFromEntity(StrongEntityPtr entity)
	{
		if(entity->has<LightComponent>() && entity->has<RenderComponent>())
		{
			glm::mat4 transform = entity->getComponent<TransformComponent>().lock()->getLocalTransform();
			std::weak_ptr<Light> light = entity->getComponent<LightComponent>().lock()->getLight();

			std::shared_ptr<LightNode> lightNode = std::make_shared<LightNode>(LightNode(getNextNodeID(), transform, entity->getName().append(" light"), light));

			std::weak_ptr<Material> material = entity->getComponent<RenderComponent>().lock()->getMaterial();
			std::shared_ptr<MaterialNode> materialNode = std::make_shared<MaterialNode>(MaterialNode(getNextNodeID(), transform, entity->getName().append(" material"), material));

			std::weak_ptr<Mesh> mesh = entity->getComponent<RenderComponent>().lock()->getMesh();
			Primitive primitive = entity->getComponent<RenderComponent>().lock()->getPrimitive();
			std::shared_ptr<GeometryNode> geometryNode = std::make_shared<GeometryNode>(GeometryNode(getNextNodeID(), transform, entity->getName().append( " geometry"), mesh, primitive));

			materialNode->addChild(geometryNode);
			lightNode->addChild(materialNode);

			return lightNode;
		}
		else if (entity->has<RenderComponent>())
		{
			glm::mat4 transform = entity->getComponent<TransformComponent>().lock()->getLocalTransform();

			std::weak_ptr<Material> material = entity->getComponent<RenderComponent>().lock()->getMaterial();
			std::shared_ptr<MaterialNode> materialNode = std::make_shared<MaterialNode>(MaterialNode(getNextNodeID(), transform, entity->getName().append(" material"), material));

			std::weak_ptr<Mesh> mesh = entity->getComponent<RenderComponent>().lock()->getMesh();
			Primitive primitive = entity->getComponent<RenderComponent>().lock()->getPrimitive();
			std::shared_ptr<GeometryNode> geometryNode = std::make_shared<GeometryNode>(GeometryNode(getNextNodeID(), transform, entity->getName().append(" geometry"), mesh, primitive));

			materialNode->addChild(geometryNode);

			return materialNode;
		}
		else if(entity->has<LightComponent>())
		{
			glm::mat4 transform = entity->getComponent<TransformComponent>().lock()->getLocalTransform();
			std::weak_ptr<Light> light = entity->getComponent<LightComponent>().lock()->getLight();

			std::shared_ptr<LightNode> lightNode = std::make_shared<LightNode>(LightNode(getNextNodeID(), transform, entity->getName().append( "light"), light));

			return lightNode;
		}
		else
		{
			glm::mat4 transform = entity->getComponent<TransformComponent>().lock()->getLocalTransform();
			std::shared_ptr<EmptyNode> emptyNode = std::make_shared<EmptyNode>(EmptyNode(getNextNodeID(), transform, entity->getName()));

			return emptyNode;
		}			
	}
}