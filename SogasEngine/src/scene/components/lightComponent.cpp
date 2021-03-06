
#include "sgspch.h"
#include "lightComponent.h"
#include "../serializerUtils.h"

namespace Sogas
{
	const char* LightComponent::s_name = "LightComponent";

	LightComponent::LightComponent()
	{
		m_light = std::make_shared<Light>();
	}

	bool LightComponent::init()
	{
		return true;
	}

	void LightComponent::to_json(json& j)
	{
		json color;
		Sogas::to_json(color, getColor());

		j["Color"]			= color;
		j["MaxDistance"]	= getMaxDistance();
		j["Intensity"]		= getIntensity();
	}

	void LightComponent::from_json(const json& jsonLight)
	{
		if (jsonLight.contains("Color") && !jsonLight["Color"].is_null())
		{
			auto jsonColor = jsonLight["Color"];

			f32 r = jsonColor.contains("x") ? jsonColor["x"].get<f32>() : 0.0f;
			f32 g = jsonColor.contains("y") ? jsonColor["y"].get<f32>() : 0.0f;
			f32 b = jsonColor.contains("z") ? jsonColor["z"].get<f32>() : 0.0f;

			setColor(glm::vec3(r, g, b));
		}
	}
}