#pragma once

#include "defines.h"

#include <../external/glm/glm/vec3.hpp>
#include <../external/glm/glm/mat4x4.hpp>
#include "types.h"
#include "components/entityComponent.h"
#include <string>
#include <memory>
#include <map>

namespace Sogas 
{
	// TODO: This is temporal until we implement an Entity Component System

	class SGS Entity {
	public:
		typedef std::map<ComponentId, StrongEntityComponentPtr> EntityComponentsMap;

		std::string m_name;

	private:
		std::string m_name;
		EntityId m_id{ 0 }; // default ID, it must count as an invalid ID
		EntityComponentsMap m_components{};
		std::string m_type;

	public:
		Entity() = default; // TODO: comprovar que aixo es correcte
		explicit Entity(EntityId id);
		Entity(const Entity& other) = default;
		~Entity(void);

		bool init();
		void postInit();
		void destroy();
		void update(f32 dt);

		std::string getName() const { return m_name; }
		EntityId getId() const { return m_id; }
		std::string getType() const { return m_type; }

		void setName(std::string name) { m_name = name; }

		template<class ComponentType>
		std::weak_ptr<ComponentType> getComponent(ComponentId id)
		{
			EntityComponentsMap::iterator findIt = m_components.find(id);
			if (findIt != m_components.end())
			{
				// Downcast and return value
				StrongEntityComponentPtr pBase(findIt->second);
				std::shared_ptr<ComponentType> pSub(std::static_pointer_cast<ComponentType>(pBase));
				std::weak_ptr<ComponentType> pWeakSub(pSub);
				return pWeakSub;
			}
			else
			{
				return std::weak_ptr<ComponentType>();
			}
		}

		template<class ComponentType>
		std::weak_ptr<ComponentType> getComponent(const char* name)
		{
			ComponentId id = EntityComponent::getIdFromName(name);
			EntityComponentsMap::iterator findIt = m_components.find(id);
			if (findIt != m_components.end())
			{
				// Downcast and return value
				StrongEntityComponentPtr pBase(findIt->second);
				std::shared_ptr<ComponentType> pSub(std::static_pointer_cast<ComponentType>(pBase));
				std::weak_ptr<ComponentType> pWeakSub(pSub);
				return pWeakSub;
			}
			else {
				return std::weak_ptr<ComponentType>();
			}
		}

		bool has(const char* componentName)
		{
			ComponentId id = EntityComponent::getIdFromName(componentName);
			EntityComponentsMap::iterator findIt = m_components.find(id);
			if (findIt != m_components.end())
				return true;
			return false;
		}

		bool has(ComponentId id)
		{
			EntityComponentsMap::iterator findIt = m_components.find(id);
			if (findIt != m_components.end())
				return true;
			return false;
		}

		const EntityComponentsMap* getComponents() { return &m_components; }
		const std::vector<StrongEntityComponentPtr>& getComponentsVector();
		void addComponent(StrongEntityComponentPtr pComponent);
		void removeComponent(const char* componentName);

		operator bool() const { return m_id != 0; }

		bool operator==(const Entity& other) const
		{
			return m_id == other.m_id; // TODO: they must be in the same scene too
		}

		bool operator !=(const Entity& other) const
		{
			return !(*this == other);
		}
	};
}