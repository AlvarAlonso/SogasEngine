#pragma once

#include "defines.h"

#include "types.h"
#include <string>
#include <memory>

namespace Sogas 
{
	// TODO: This is temporal until we implement an Entity Component System

	class SGS Entity {
	public:

		typedef std::map<ComponentId, StrongEntityComponentPtr> EntityComponents;

	private:
		EntityId m_id;
		EntityComponents m_components{};
		std::string m_type;

	public:
		explicit Entity(EntityId id);
		~Entity(void);

		bool init();
		void postInit();
		void destroy();
		void update(f32 dt);

		EntityId getId() const { return m_id; }
		std::string getType() const { return m_type; }

		template<class ComponentType>
		std::weak_ptr<ComponentType> getComponent(ComponentId id)
		{
			EntityComponents::iterator findIt = m_components.find(id);
			if (findIt != m_components.end())
			{
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

		// TODO: Retrieve from const char* name
		template<class componentType>
		std::weak_ptr<componentType> getComponent(const char* name)
		{
			//componentId id = actorComponents::getid
			return std::weak_ptr<componentType>();
		}

		const EntityComponents* getComponents() { return &m_components; }
		void addComponent(StrongEntityComponentPtr pComponent);
	};
}