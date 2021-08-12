#include "sgspch.h"
#include "entityScriptComponent.h"

namespace Sogas
{
	const char* EntityScriptComponent::s_name = "EntityScriptComponent";

	EntityScriptComponent::EntityScriptComponent(EntityScript* script)
	{
		m_entityScript = script;
	}

	bool EntityScriptComponent::init()
	{
		return false;
	}
}