#include "sgspch.h"
#include "entityScriptComponent.h"

namespace Sogas
{
	const char* EntityScriptComponent::s_name = "EntityScriptComponent";

	bool EntityScriptComponent::init()
	{
		m_entityScript = new EntityScript();
		m_entityScript->createFromScript("TestScript");
		m_entityScript->start();

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