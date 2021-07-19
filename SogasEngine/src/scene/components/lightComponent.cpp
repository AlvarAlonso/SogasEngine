
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

	//void LightComponent::to_json(json& j)
	//{
	//	json color;
	//	Sogas::to_json(color, getColor());

	//	j = json
	//	{
	//		{ "Color", color },
	//		{ "MaxDistance", getMaxDistance() },
	//		{ "Intensity", getIntensity() }
	//	};
	//}
}