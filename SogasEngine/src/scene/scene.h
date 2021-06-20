#pragma once

#include "defines.h"
#include "sgspch.h"
#include "entity.h"

namespace Sogas {

	class Scene
	{
	public:
		Scene() = default;

		Entity createEntity(const std::string& name = std::string());
		void DestroyEntity(Entity entity);

		std::vector<Entity>& getEntities() { return m_entities; }

	private:
		std::vector<Entity> m_entities;
	};
}