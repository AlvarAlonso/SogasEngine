#include "sgspch.h"
#include "entityScriptComponent.h"

namespace Sogas
{
	const char* EntityScriptComponent::s_name = "EntityScriptComponent";

	/*
	* @brief Construct the entity script component. It is implemented as an empty component and 
	* later the user will fill it up loading the necessary script.
	* @param void
	* @return bool
	*/
	bool EntityScriptComponent::init()
	{
		m_entityScript = nullptr;

		return true;
	}

	void EntityScriptComponent::postInit(void)
	{
	}

	void EntityScriptComponent::to_json(json& j)
	{
		// TODO
	}

	void EntityScriptComponent::from_json(const json& j)
	{
		// TODO
	}

	/*
	* @brief Receive the filename path. Creates an Entity Script if it does not exist and init it from
	* the file being passed.
	* @param const char* filename
	* @return void
	*/
	void EntityScriptComponent::setEntityScript(const char* filename)
	{
		if (!m_entityScript)
			m_entityScript = new EntityScript();

		m_entityScript->createFromScript(filename);
		m_entityScript->setOwner(this);
		m_entityScript->start();
	}
}