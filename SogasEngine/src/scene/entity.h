#pragma once

#include "defines.h"

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include "types.h"
#include "components/entityComponent.h"
#include <string>
#include <memory>
#include <map>

namespace Sogas 
{
	class SGS Entity {
	public:
		typedef std::map<ComponentId, StrongEntityComponentPtr> EntityComponentsMap;

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

		template <class T>
		std::weak_ptr<T> getComponent()
		{
			ComponentId id = EntityComponent::getIdFromName(T::s_name);
			EntityComponentsMap::iterator findIt = m_components.find(id);
			if (findIt != m_components.end())
			{
				// Downcast and return value
				StrongEntityComponentPtr pBase(findIt->second);
				std::shared_ptr<T> pSub(std::static_pointer_cast<T>(pBase));
				std::weak_ptr<T> pWeakSub(pSub);
				return pWeakSub;
			}
			else {
				return std::weak_ptr<T>();
			}
		}

		template <class T>
		bool has()
		{
			ComponentId id = EntityComponent::getIdFromName(T::s_name);
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