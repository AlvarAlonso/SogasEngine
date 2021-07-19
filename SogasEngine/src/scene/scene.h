#pragma once

#include "defines.h"

#include "entity.h"
#include "entityFactory.h"
#include "nlohmann/json.hpp"

namespace Sogas 
{
	using json = nlohmann::json;

	class SGS Scene
	{
	public:
		Scene();
		Scene(const char* filename);
		~Scene();

		StrongEntityPtr createEntity(const std::string& name = std::string());
		void destroyEntity(EntityId entityId);
		void onUpdate(f32 dt);

		void addComponent(StrongEntityPtr entity, const char* componentName);
		const std::vector<StrongEntityPtr>& getEntities() { return m_entities; };
		StrongEntityPtr findEntityById(EntityId entityId);

		void to_json(json& j)
		{
			json entArray = json::array();
			for (auto& entity : getEntities())
			{
				json ent;
				//entity->to_json(ent);
				entArray.push_back(ent);
			}
			j = json{ {"Entities", entArray} };
		}

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