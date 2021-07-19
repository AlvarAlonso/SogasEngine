
#include "sgspch.h"
#include "transformComponent.h"
#include "scene/serializerUtils.h"

namespace Sogas
{
	const char* TransformComponent::s_name = "TransformComponent";

	bool TransformComponent::init()
	{
		m_translation = glm::vec3();
		m_rotation = glm::vec3();
		m_scale = glm::vec3(1);
		return true;
	}

	void TransformComponent::to_json(json& j)
	{
		json translation, rotation, scale;
		Sogas::to_json(translation, getTranslation());
		Sogas::to_json(rotation, getRotation());
		Sogas::to_json(scale, getScale());

		j = json{
			{"Translation", translation},
			{"Rotation", rotation},
			{"Scale", scale}
		};
	}
}
