
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

	void TransformComponent::from_json(const json& jsonTransform)
	{
		if (jsonTransform.contains("Translation") && !jsonTransform["Translation"].is_null())
		{
			auto jsonPosition = jsonTransform["Translation"];

			f32 x = jsonPosition.contains("x") ? jsonPosition["x"].get<f32>() : 0.0f;
			f32 y = jsonPosition.contains("y") ? jsonPosition["y"].get<f32>() : 0.0f;
			f32 z = jsonPosition.contains("z") ? jsonPosition["z"].get<f32>() : 0.0f;

			glm::vec3 position = glm::vec3(x, y, z);
			setTranslation(position);
		}
		if (jsonTransform.contains("Rotation") && !jsonTransform["Rotation"].is_null())
		{
			auto jsonRotation = jsonTransform["Rotation"];

			f32 x = jsonRotation.contains("x") ? jsonRotation["x"].get<f32>() : 0.0f;
			f32 y = jsonRotation.contains("y") ? jsonRotation["y"].get<f32>() : 0.0f;
			f32 z = jsonRotation.contains("z") ? jsonRotation["z"].get<f32>() : 0.0f;
			//f32 w = jsonRotation.contains("w") ? jsonRotation["w"].get<f32>() : 0.0f;

			// TODO: Add support for quaternions

			//glm::vec3 rotation = glm::eulerAngles(glm::quat(x, y, z, w));
			glm::vec3 rotation = glm::vec3(x, y, z);
			setRotation(rotation);
		}
		if (jsonTransform.contains("Scale") && !jsonTransform["Scale"].is_null())
		{
			auto jsonScale = jsonTransform["Scale"];

			f32 x = jsonScale.contains("x") ? jsonScale["x"].get<f32>() : 0.0f;
			f32 y = jsonScale.contains("y") ? jsonScale["y"].get<f32>() : 0.0f;
			f32 z = jsonScale.contains("z") ? jsonScale["z"].get<f32>() : 0.0f;

			glm::vec3 scale = glm::vec3(x, y, z);
			setScale(scale);
		}
	}
}
