
#include "sgspch.h"
#include "lightComponent.h"
#include "../serializerUtils.h"

namespace Sogas
{
	const char* LightComponent::s_name = "LightComponent";

	bool LightComponent::init()
	{
		return true;
	}

	void LightComponent::to_json(json& j)
	{
		json color;
		Sogas::to_json(color, getColor());

		j = json
		{
			{ "Color", color },
			{ "MaxDistance", getMaxDistance() },
			{ "Intensity", getIntensity() }
		};
	}

	void LightComponent::from_json(const json& jsonLight)
	{
		if (jsonLight.contains("Color") && !jsonLight["Color"].is_null())
		{
			auto jsonColor = jsonLight["Color"];

			f32 r = jsonColor.contains("r") ? jsonColor["r"].get<f32>() : 0.0f;
			f32 g = jsonColor.contains("g") ? jsonColor["g"].get<f32>() : 0.0f;
			f32 b = jsonColor.contains("b") ? jsonColor["b"].get<f32>() : 0.0f;

			setColor(glm::vec3(r, g, b));
		}
	}
}