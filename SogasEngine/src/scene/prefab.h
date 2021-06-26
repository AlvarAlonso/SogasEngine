#pragma once

#include "defines.h"

#include <../external/glm/glm/gtc/matrix_transform.hpp>

#include <string>
#include <memory>
#include <vector>
#include <map>

namespace Sogas 
{
	class Mesh;

	class SGS Node
	{
	public:
		std::string m_name;
		bool m_visible;

		std::shared_ptr<Mesh> m_mesh;
		glm::mat4 m_model;
		glm::mat4 m_globalModel;

		// TODO: not sure about the type of the pointer
		std::shared_ptr<Node> m_parent;
		std::vector<std::shared_ptr<Node>> m_children;

		Node();

		virtual ~Node();

		// add node to children list
		void addChild(Node* child);
		glm::mat4 getGlobalMatrix(bool fast = false);

		// TODO: While no ECS user render func
		void render();
	};

	class SGS Prefab
	{
	public:
		std::string m_name;
		std::map<std::string, std::weak_ptr<Node>> m_nodesByName;

		std::vector<std::weak_ptr<Node>> m_roots;

		Prefab() = default;
		virtual ~Prefab();

		// manager to cache loaded prefabs
		static std::map<std::string, std::shared_ptr<Prefab>> s_prefabsLoaded;
		static std::shared_ptr<Prefab> get(const std::string& name);
		void registerPrefab(const std::string& name);

		// TODO: while no ECS, use render func
		void render();
	};
}