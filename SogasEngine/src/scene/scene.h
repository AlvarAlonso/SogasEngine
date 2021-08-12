#pragma once

#include "defines.h"

#include "entity.h"
#include "entityFactory.h"
#include "environment.h"
#include <json/single_include/nlohmann/json.hpp>

namespace Sogas 
{
	using json = nlohmann::json;

	class Environment;

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
		std::weak_ptr<Environment> getEnvironment() { return m_pEnvironment; }

		StrongEntityPtr findEntityById(EntityId entityId);

		void to_json(json& j);
		void from_json(const json& j);

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
			StrongEntityComponentPtr pComponent = m_pEntityFactory->createComponent(T::s_name);
			entity->addComponent(pComponent);
			pComponent->setOwner(entity);
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

	public:
		bool m_renderEnvironment{ true };

	private:
		std::vector<StrongEntityPtr> m_entities;
		std::unique_ptr<EntityFactory> m_pEntityFactory;
		std::shared_ptr<Environment> m_pEnvironment;
	};

}