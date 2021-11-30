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

		// add entity groups to the node map
		SceneNodeList children = m_root->getChildren();
		for(size_t i = 0; i < children.size(); i++)
		{
			NodeId nodeId = children[i]->getNodeProperties()->getNodeId();
			m_nodeMap[nodeId] = children[i];
		}
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
		{
			SGSDEBUG("NodeId not found in node map!");
			return std::shared_ptr<ISceneNode>();
		}

		return (*it).second;
	}

	std::shared_ptr<ISceneNode> SceneGraph::findNode(EntityId entityId, SceneNodeType nodeType)
	{
		EntityToSceneNodesMap::iterator entityToSceneNodesIt = m_entityNodesMap.find(entityId);
		if (entityToSceneNodesIt == m_entityNodesMap.end())
		{
			SGSDEBUG("The entity id specified has not been found in the EntityToSceneNodesMap");
			return nullptr;
		}

		TypeNodeMap::iterator typeNodeIt = entityToSceneNodesIt->second.find(nodeType);
		if(typeNodeIt == entityToSceneNodesIt->second.end())
		{
			SGSDEBUG("The node type specified has not been found in the type to scene node map");
			return nullptr;
		}

		return typeNodeIt->second;
	}

	std::shared_ptr<ISceneNode> SceneGraph::findLastEntityNode(EntityId entityId, SceneNodeType nodeType)
	{
		u32 type = 0;

		// if the node is empty, its parent must be a node from another entity
		if(nodeType == SceneNodeType::TRANSFORM)
		{
			type = (u32)(u32(SceneNodeType::LAST));
		}
		else
		{
			type = (u32)((u32)nodeType - 1);
		}

		for(type; type >= (u32)SceneNodeType::TRANSFORM; type--)
		{
			std::shared_ptr<ISceneNode> lastNode = findNode(entityId, (SceneNodeType)type);

			if(lastNode)
			{
				return lastNode;
			}
		}

		return nullptr;
	}

	bool SceneGraph::addChild(NodeId id, std::shared_ptr<ISceneNode> child)
	{
		child->getNodeProperties()->getType();

		return m_root->addChild(child);
	}

	bool SceneGraph::registerNodeToMaps(std::shared_ptr<ISceneNode> node)
	{
		EntityId entityId = node->getNodeProperties()->getEntityId();
		NodeId nodeId = node->getNodeProperties()->getNodeId();

		// if it is a new entity we have to add it to the entity to scene nodes map
		if(node->getNodeProperties()->getType() == SceneNodeType::TRANSFORM)
		{
			TypeNodeMap typeNodeMap;
			m_entityNodesMap.insert({ entityId, typeNodeMap });
		}

		// map of all nodes
		m_nodeMap[nodeId] = node;

		// map of type nodes an entity has
		if (m_entityNodesMap.find(entityId) != m_entityNodesMap.end())
		{
			TypeNodeMap& typeToNode = m_entityNodesMap[entityId];
			typeToNode.insert({ node->getNodeProperties()->getType(), node });

			return true;
		}
		else
		{
			SGSERROR("The entityId has not been found in the entityNodesMap!");
			return false;
		}
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

	void SceneGraph::updateNode(EntityId entityId, SceneNodeType nodeType, void* data)
	{
		// TODO: refactor this function with the findNode method

		EntityToSceneNodesMap::iterator entityToSceneNodesIt = m_entityNodesMap.find(entityId);
		if (entityToSceneNodesIt == m_entityNodesMap.end())
		{
			SGSDEBUG("The entity id specified has not been found in the EntityToSceneNodesMap");
			return;
		}
		TypeNodeMap::iterator typeNodeIt = entityToSceneNodesIt->second.find(nodeType);
		if (typeNodeIt == entityToSceneNodesIt->second.end())
		{
			SGSDEBUG("The node type specified has not been found in the type to scene node map");
			return;
		}

		typeNodeIt->second->updateNode(data);
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

	bool SceneGraph::placeNode(std::shared_ptr<SceneNode> node, EntityId entityId, EntityId parentId)
	{
		SceneNodeType nodeType = node->getNodeProperties()->getType();

		// if the node is empty and it has parent
		if (parentId == 0 && nodeType == SceneNodeType::TRANSFORM) // TODO: change the way to access node properties because it looks shitty as fuck
		{
			// TODO: This must be changed. It does not child the node to the node of the id you pass, it just childs it to a root node depending
			// on the render pass. The id of the node needs to be passed
			NodeId nodeId = node->getNodeProperties()->getNodeId();
			return addChild(nodeId, node);
		}
		else
		{
			std::shared_ptr<ISceneNode> lastNode = nullptr;

			if(nodeType == SceneNodeType::TRANSFORM)
			{
				// search the corresponding place in the nodes belonging to the parent entity
				lastNode = findLastEntityNode(parentId, nodeType);
			}
			else
			{
				// search the corresponding place in the nodes belonging to the entity
				lastNode = findLastEntityNode(entityId, nodeType);
			}

			SceneNodeList lastNodeChildren = lastNode->getChildren();
			for (size_t i = 0; i < lastNodeChildren.size(); i++)
			{
				lastNode->removeChild(lastNodeChildren[i]->getNodeProperties()->getNodeId());
				node->addChild(lastNodeChildren[i]);
			}

			// the new node is placed as a child of the last node of the parent.
			return lastNode->addChild(node);
		}
		
		return false;
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
		// TODO: refactor this hardcoded mess

		EntityId entityId = entity->getId();
		std::shared_ptr<TransformNode> transformNode = std::make_shared<TransformNode>(TransformNode(getNextNodeID(), entity->getName(), entityId));

		if(entity->has<LightComponent>() && entity->has<RenderComponent>())
		{
			std::weak_ptr<Light> light = entity->getComponent<LightComponent>().lock()->getLight();

			std::shared_ptr<LightNode> lightNode = std::make_shared<LightNode>(LightNode(getNextNodeID(), entity->getName().append(LightNode::getStaticName()), light, entityId));

			std::weak_ptr<Material> material = entity->getComponent<RenderComponent>().lock()->getMaterial();
			std::shared_ptr<MaterialNode> materialNode = std::make_shared<MaterialNode>(MaterialNode(getNextNodeID(), entity->getName().append(MaterialNode::getStaticName()), material, entityId));

			std::weak_ptr<Mesh> mesh = entity->getComponent<RenderComponent>().lock()->getMesh();
			Primitive primitive = entity->getComponent<RenderComponent>().lock()->getPrimitive();
			std::shared_ptr<GeometryNode> geometryNode = std::make_shared<GeometryNode>(GeometryNode(getNextNodeID(), entity->getName().append(GeometryNode::getStaticName()), mesh, primitive, entityId));

			materialNode->addChild(geometryNode);
			lightNode->addChild(materialNode);
			transformNode->addChild(lightNode);

			TypeNodeMap* typeToNode = new TypeNodeMap;
			typeToNode->insert({ SceneNodeType::TRANSFORM, transformNode });
			typeToNode->insert({ SceneNodeType::LIGHT, lightNode });
			typeToNode->insert({ SceneNodeType::MATERIAL, materialNode });
			typeToNode->insert({ SceneNodeType::GEOMETRY, geometryNode });

			m_nodeMap.insert({ transformNode->getNodeProperties()->getNodeId(), transformNode });
			m_nodeMap.insert({ lightNode->getNodeProperties()->getNodeId(), lightNode });
			m_nodeMap.insert({ materialNode->getNodeProperties()->getNodeId(), materialNode });
			m_nodeMap.insert({ geometryNode->getNodeProperties()->getNodeId(), geometryNode });

			m_entityNodesMap.insert({ entity->getId(), *typeToNode });

			typeToNode = nullptr;
		}
		else if (entity->has<RenderComponent>())
		{
			std::weak_ptr<Material> material = entity->getComponent<RenderComponent>().lock()->getMaterial();
			std::shared_ptr<MaterialNode> materialNode = std::make_shared<MaterialNode>(MaterialNode(getNextNodeID(), entity->getName().append(MaterialNode::getStaticName()), material, entityId));

			std::weak_ptr<Mesh> mesh = entity->getComponent<RenderComponent>().lock()->getMesh();
			Primitive primitive = entity->getComponent<RenderComponent>().lock()->getPrimitive();
			std::shared_ptr<GeometryNode> geometryNode = std::make_shared<GeometryNode>(GeometryNode(getNextNodeID(), entity->getName().append(GeometryNode::getStaticName()), mesh, primitive, entityId));

			materialNode->addChild(geometryNode);
			transformNode->addChild(materialNode);

			TypeNodeMap* typeToNode = new TypeNodeMap;
			typeToNode->insert({ SceneNodeType::TRANSFORM,  transformNode });
			typeToNode->insert({ SceneNodeType::MATERIAL, materialNode });
			typeToNode->insert({ SceneNodeType::GEOMETRY, geometryNode });

			m_nodeMap.insert({ transformNode->getNodeProperties()->getNodeId(), transformNode });
			m_nodeMap.insert({ materialNode->getNodeProperties()->getNodeId(), materialNode });
			m_nodeMap.insert({ geometryNode->getNodeProperties()->getNodeId(), geometryNode });

			m_entityNodesMap.insert({ entity->getId(), *typeToNode });
			typeToNode = nullptr;
		}
		else if(entity->has<LightComponent>())
		{
			std::weak_ptr<Light> light = entity->getComponent<LightComponent>().lock()->getLight();

			std::shared_ptr<LightNode> lightNode = std::make_shared<LightNode>(LightNode(getNextNodeID(), entity->getName().append(LightNode::getStaticName()), light, entityId));

			transformNode->addChild(lightNode);

			TypeNodeMap* typeToNode = new TypeNodeMap;
			typeToNode->insert({ SceneNodeType::TRANSFORM, transformNode });
			typeToNode->insert({ SceneNodeType::LIGHT, lightNode });

			m_nodeMap.insert({ transformNode->getNodeProperties()->getNodeId(), transformNode });
			m_nodeMap.insert({ lightNode->getNodeProperties()->getNodeId(), lightNode });

			m_entityNodesMap.insert({ entity->getId(), *typeToNode });
			typeToNode = nullptr;
		}
		else
		{
			TypeNodeMap* typeToNode = new TypeNodeMap;
			typeToNode->insert({ SceneNodeType::TRANSFORM , transformNode });

			m_entityNodesMap.insert({ entity->getId(), *typeToNode });

			m_nodeMap.insert({ transformNode->getNodeProperties()->getNodeId(), transformNode });

			typeToNode = nullptr;
		}

		return transformNode;
	}
}