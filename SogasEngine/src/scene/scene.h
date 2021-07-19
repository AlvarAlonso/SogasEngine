#pragma once

#include "defines.h"

#include "entity.h"
#include "entityFactory.h"

namespace Sogas 
{

	class SGS Scene
	{
	public:
		Scene();
		Scene(const char* filename);
		~Scene();

		StrongEntityPtr createEntity(const std::string& name = std::string());
		void destroyEntity(EntityId entityId);
		void destroy();
		void onUpdate(f32 dt);

		void addComponent(StrongEntityPtr entity, const char* componentName);
		const std::vector<StrongEntityPtr>& getEntities() { return m_entities; };
		StrongEntityPtr findEntityById(EntityId entityId);

		template <class T>
		std::vector<StrongEntityPtr> getByComponent()
		{
			std::vector<StrongEntityPtr> returnVector{};

			for (auto& entity : m_entities)
			{
				if (entity->has<T>())
				{
					returnVector.push_back(entity);
				}
			}

			return returnVector;
		}
	private:
		std::vector<StrongEntityPtr> m_entities;
		std::unique_ptr<EntityFactory> m_pEntityFactory;
	};

}