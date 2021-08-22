#include "sgspch.h"
#include "entityScriptComponent.h"

namespace Sogas
{
	const char* EntityScriptComponent::s_name = "EntityScriptComponent";

	bool EntityScriptComponent::init()
	{
		m_entityScript = new EntityScript();
		// TODO: this is hardcoded
		m_entityScript->createFromScript("TestScript");

		return true;
	}

	void EntityScriptComponent::postInit(void)
	{
		m_entityScript->setOwner(this);
		m_entityScript->start();
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