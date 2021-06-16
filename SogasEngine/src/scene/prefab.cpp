#include "prefab.h"

#include "core/assertions.h"

namespace Sogas 
{
	std::map<std::string, std::shared_ptr<Prefab>> Prefab::s_prefabsLoaded;

	Node::Node() : m_visible(true), m_parent(nullptr), m_mesh(nullptr)
	{
	}

	Node::~Node()
	{
	}

	void Node::addChild(Node* child)
	{
		SGSASSERT(child->m_parent == nullptr);

		std::shared_ptr<Node> childToAdd;
		childToAdd = std::make_shared<Node>();
		childToAdd.reset(child);

		m_children.push_back(childToAdd);

		child->m_parent.reset(this);
	}

	glm::mat4 Node::getGlobalMatrix(bool fast)
	{
		if (m_parent)
			m_globalModel = m_model * (fast ? m_parent->m_globalModel : m_parent->getGlobalMatrix());
		else
			m_globalModel = m_model;
		return m_globalModel;
	}

	Prefab::~Prefab()
	{
		SGSASSERT(m_name.size());

		auto it = s_prefabsLoaded.find(m_name);
		if (it != s_prefabsLoaded.end());
		s_prefabsLoaded.erase(it);
	}

	std::shared_ptr<Prefab> Prefab::get(const std::string& name)
	{
		SGSASSERT(name.size());

		std::map<std::string, std::shared_ptr<Prefab>>::iterator it = s_prefabsLoaded.find(name);
		if (it != s_prefabsLoaded.end())
			return it->second;

		// TODO: else, load the prefab file

		return nullptr;
	}

	void Prefab::registerPrefab(const std::string& name)
	{
		m_name = name;
		s_prefabsLoaded[name].reset(this);
	}
}