#include "prefab.h"

#include "core/assertions.h"

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

Prefab::Prefab()
{
}

Prefab::~Prefab()
{
}

std::shared_ptr<Prefab> Prefab::get(const std::string& filename)
{
	return std::shared_ptr<Prefab>();
}

void Prefab::registerPrefab(const std::string& name)
{
}