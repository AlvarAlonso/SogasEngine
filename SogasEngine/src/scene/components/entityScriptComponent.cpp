#include "sgspch.h"
#include "entityScriptComponent.h"

namespace Sogas
{
	const char* EntityScriptComponent::s_name = "EntityScriptComponent";

	bool EntityScriptComponent::init()
	{
		return true;
	}

	void EntityScriptComponent::to_json(json& j)
	{
		// TODO
	}

	void EntityScriptComponent::from_json(const json& j)
	{
		// TODO
	}
}