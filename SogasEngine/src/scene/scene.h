#pragma once

#include "defines.h"

#include "entity.h"
#include "entityFactory.h"
//#include <nlohmann/json_fwd.hpp>

namespace Sogas 
{
	//using json = nlohmann::json;
	//class nlohmann::json;

	class SGS Scene
	{
	public:
		Scene();
		Scene(const char* filename);
		~Scene();

		StrongEntityPtr createEntity(const std::string& name = std::string());
		void destroyEntity(EntityId entityId);
		void onUpdate(f32 dt);

		const std::vector<StrongEntityPtr>& getEntities() { return m_entities; };
		StrongEntityPtr findEntityById(EntityId entityId);

		//void to_json(json& j);

		/*
		* Adds component to an entity
		* @param StrongEntityPtr
		* @return void
		*/
		template <class T>
		void addComponent(StrongEntityPtr entity)
		{
			//if (!T:s_name)
			//{
			//	SGSWARN("No valid component name given.");
			//	return;
			//}

			// TODO: name from type may not be the same as stored in static s_name
			// Should find a more robust way to create a specific component type
			entity->addComponent(m_pEntityFactory->createComponent(T::s_name));
		}

		/*
		* Get all entities with a given component
		* @param void
		* @return std::vector<StrongEntityPtr>
		*/
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