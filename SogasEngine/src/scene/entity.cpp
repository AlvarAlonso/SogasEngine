#include "entity.h"

namespace Sogas 
{
	Entity::Entity() : m_model(glm::mat4(1))
	{
	}

	Renderable::Renderable() : Entity()
	{
	}

	Renderable::Renderable(const std::string& name) : Entity()
	{
		m_name = name;
	}

	Light::Light() : Entity()
	{
	}

	Light::Light(const std::string& name) : Entity()
	{
		m_name = name;
	}
}
