#pragma once

#include <memory>
#include <string>
#include <glm/glm/mat4x4.hpp>
#include "scene/types.h"

namespace Sogas
{
	class Environment;
	class Material;
	class Mesh;
	class Light;
	class Camera;
	class SceneGraph;

	enum class MainRenderPass
	{
		_0 = 2, // 0 is invalid, 1 is root node
		OPAQUE = _0,
		TRANSPARENT = 3,
		ENVIRONMENT = 4,
		LAST // invalid location (like an iterator.end())
	};

	struct SceneNodeProperties
	{
	private:
		friend class SceneNode;

	protected:
		NodeId nodeId; // TODO: 0 is invalid, 1 is root, the others are available as an ID for the other nodes
		EntityId entityId;
		SceneNodeType type;
		glm::mat4 transform;
		float radius;
		std::string name; // debug purposes
		MainRenderPass renderPass;
	public:
		const NodeId& getNodeId() const { return nodeId; }
		const EntityId& getEntityId() const { return entityId; }
		const SceneNodeType& getType() const { return type; }
		glm::mat4 const& getTransform() const { return transform; }
		float getRadius() const { return radius; }
		MainRenderPass getMainRenderPass() const { return renderPass; }
		void setMainRenderPass(MainRenderPass newRenderPass) { renderPass = newRenderPass; }
		void setName(std::string newName) { name = newName; }
		void setType(SceneNodeType newType) { type = newType; }
	};

	class SGS ISceneNode
	{
	public:
		virtual const SceneNodeProperties* const getNodeProperties() const = 0;

		virtual SceneNodeList getChildren() const = 0;
		virtual void setTransform(glm::mat4 transform) = 0;
		virtual void setRenderPass(MainRenderPass renderPass) = 0;
		virtual bool hasChildren() = 0;

		virtual void preRender(SceneGraph* pScene) = 0;
		virtual bool isVisible(SceneGraph* pScene) const = 0;
		virtual void render(SceneGraph* pScene) = 0;
		virtual void renderChildren(SceneGraph* pScene) = 0;
		virtual void postRender(SceneGraph* pScene) = 0;
	
		virtual bool addChild(std::shared_ptr<ISceneNode> child) = 0;
		virtual bool removeChild(NodeId id) = 0;

		virtual void updateNode(void* data) = 0;
		virtual void updateNodeTransform(void* data) = 0;

		virtual ~ISceneNode() {};
	};

	class SGS SceneNode : public ISceneNode
	{
	protected:
		SceneNodeList m_children;
		std::weak_ptr<SceneNode> m_parent;
		SceneNodeProperties m_properties;
	public:
		SceneNode(const NodeId nodeId, const glm::mat4 transform, const std::string name, const EntityId entityId = 0);
		virtual ~SceneNode();
		virtual const SceneNodeProperties* const getNodeProperties() const override { return &m_properties; };
		virtual SceneNodeList getChildren() const { return m_children; };
		virtual void setTransform(glm::mat4 transform) override;
		virtual void setRenderPass(MainRenderPass renderPass) override { m_properties.renderPass = renderPass; };
		virtual bool hasChildren() { return !m_children.empty(); };

		virtual void preRender(SceneGraph* pScene) override;
		virtual bool isVisible(SceneGraph* pScene) const override;
		virtual void render(SceneGraph* pScene) override;
		virtual void renderChildren(SceneGraph* pScene) override;
		virtual void postRender(SceneGraph* pScene) override;

		virtual bool addChild(std::shared_ptr<ISceneNode> child) override;
		virtual bool removeChild(NodeId id) override;

		virtual void updateNode(void* data) override;
		virtual void updateNodeTransform(void* data) override;
	};

	class SGS RootNode : public SceneNode
	{
	public:
		RootNode(const NodeId nodeId = 1, glm::mat4 transform = glm::mat4(1), const std::string name = "RootNode");
		static const std::string getStaticName() { return "_root"; };

		virtual bool addChild(std::shared_ptr<ISceneNode> child);
		virtual void renderChildren(SceneGraph* pScene);
		virtual bool isVisible() const { return true; }

		void resetNode();
	};

	class SGS CameraNode : public SceneNode
	{
	private:
		Camera* m_pCamera;
	public:
		CameraNode(const NodeId nodeId, const glm::mat4 transform, std::string name, const EntityId entityId = 0);
		CameraNode(const NodeId nodeId, const glm::mat4 transform, std::string name, std::weak_ptr<Camera> camera, const EntityId entityId = 0);
		static const std::string getStaticName() { return "_camera"; };

		virtual void render(SceneGraph* pScene) override;
		virtual bool isVisible(SceneGraph* pScene) const override;

		virtual void updateNode(void* data) override;

		// camera wrapper functions
		void lookAt(const glm::vec3 position, const glm::vec3 center);
		glm::mat4 getView();
		glm::mat4 getProjection();
		glm::mat4 getViewProjection();
		inline void setPosition(glm::vec3 pos);
	};

	class SGS LightNode : public SceneNode
	{
	private:
		Light* m_pLight;

	public:
		LightNode(const NodeId nodeId, const glm::mat4 transform, std::string name, const EntityId entityId = 0);
		LightNode(const NodeId nodeId, const glm::mat4 transform, std::string name, std::weak_ptr<Light> light, const EntityId entityId = 0);
		static const std::string getStaticName() { return "_light"; };

		virtual void updateNode(void* data) override;
	};

	class SGS MaterialNode: public SceneNode
	{
	private:
		Material* m_pMaterial;

	public:
		MaterialNode(const NodeId nodeId, const glm::mat4 transform, std::string name, const EntityId entityId = 0);
		MaterialNode(const NodeId nodeId, const glm::mat4 transform, std::string name, std::weak_ptr<Material> material, const EntityId entityId = 0);
		static const std::string getStaticName() { return "_material"; };

		virtual void preRender(SceneGraph* pScene) override;

		virtual void updateNode(void* data) override;
	};

	class SGS GeometryNode : public SceneNode
	{
	private:
		Mesh* m_pMesh;
		Primitive m_primitive;

	public:
		GeometryNode(const NodeId nodeId, const glm::mat4 transform, std::string name, const EntityId entityId = 0);
		GeometryNode(const NodeId nodeId, const glm::mat4 transform, const std::string name, std::weak_ptr<Mesh> mesh, Primitive primitive, const EntityId entityId = 0);
		static const std::string getStaticName() { return "_geometry"; };

		virtual void render(SceneGraph* pScene) override;

		virtual void updateNode(void* data) override;
	};

	class SGS EnvironmentNode : public SceneNode
	{
	private:
		Environment* m_pEnvironment;

	public:
		EnvironmentNode(const NodeId nodeId, const glm::mat4 transform, std::string name, const EntityId entityId = 0);
		EnvironmentNode(const NodeId nodeId, const glm::mat4 transform, const std::string name, std::weak_ptr<Environment> environment, const EntityId entityId = 0);
		static const std::string getStaticName() { return "_environment"; };

		virtual void preRender(SceneGraph* pScene) override;
		virtual void render(SceneGraph* pScene) override;

		virtual void updateNode(void* data) override;
	};

	// represents a node that just adds a transfrom in the scene graph
	// and can occlude all its children with the isVisible parameter
	class SGS EmptyNode : public SceneNode
	{
	public:
		EmptyNode(NodeId nodeId, glm::mat4 transform, std::string name, const EntityId entityId = 0);
		static const std::string getStaticName() { return "_empty"; };

		virtual void preRender(SceneGraph* pScene) override;

		virtual void updateNode(void* data) override;
	};
}