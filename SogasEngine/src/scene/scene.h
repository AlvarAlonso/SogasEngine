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
		void DestroyEntity(StrongEntityPtr entity);

		void destroy();

		void onUpdate(f32 dt);

		void addComponent(StrongEntityPtr entity, const char* componentName);
		const std::vector<StrongEntityPtr>& getEntities() { return m_entities; };
		std::vector<StrongEntityPtr> getByComponent(const char* componentName);

	private:
		std::vector<StrongEntityPtr> m_entities;
		std::unique_ptr<EntityFactory> m_pEntityFactory;
	};

}