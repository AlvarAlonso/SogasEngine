#include "prefab.h"

#include "core/assertions.h"

Node::Node()
{
}

Node::~Node()
{
}

void Node::addChild(Node* child)
{
}

glm::mat4 Node::getGlobalMatrix(bool fast)
{
	return glm::mat4();
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
