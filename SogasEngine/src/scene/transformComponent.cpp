
#include "sgspch.h"
#include "transformComponent.h"

namespace Sogas
{
	const char* TransformComponent::s_name = "TransformComponent";

	bool TransformComponent::init()
	{
		m_transform = glm::mat4(1);
		return true;
	}
}