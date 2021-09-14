#pragma once

#include "defines.h"

#include "types.h"
#include "components/entityComponent.h"

#include <json/single_include/nlohmann/json.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <string>
#include <memory>
#include <map>

namespace Sogas 
{

	using json = nlohmann::json;

	class Scene;


	/*
	* entityid
	* name
	* type
	* vector<components>
	*/

	class IEntity
	{
		virtual bool isVisible() const = 0;

		virtual void onUpdate() const = 0;
		virtual void preRender() const = 0;
		virtual void onRender() const = 0;
		virtual void postRender() const = 0;

		virtual bool addChild(std::shared_ptr<IEntity> child) = 0;
		virtual bool removeChild(EntityId id) = 0;
	};

	class SGS Entity : public std::enable_shared_from_this<Entity>
	{
	public:
		typedef std::map<ComponentId, StrongEntityComponentPtr> EntityComponentsMap;

	private:
		std::string						m_name{ "entity" };
		std::string						m_type{ "unknown" };
		EntityId						m_id{ 0 }; // default ID, it must count as an invalid ID
		EntityComponentsMap				m_components{};
		std::vector<StrongEntityPtr>	m_childs;
		Entity*							m_parent{ nullptr };
		std::shared_ptr<Scene>			m_pScene;

	public:
		explicit Entity(EntityId id);
		//Entity(const Entity& other) = default;
		~Entity(void);

		bool		init();
		void		postInit();
		void		destroy();
		void		update(f32 dt);

		void		setName(std::string name) { m_name = name; }
		std::string getName() const { return m_name; }
		EntityId	getId() const { return m_id; }
		std::string getType() const { return m_type; }
		std::weak_ptr<Scene> getScene() { return m_pScene; }
		void		setScene(std::shared_ptr<Scene> pScene) { m_pScene = pScene; }
		bool		isSelected();

		void		to_json(json& j);
		void		from_json(const json& j);

		const EntityComponentsMap* getComponents() { return &m_components; }
		const std::vector<StrongEntityComponentPtr>& getComponentsVector();	// TODO This vector returns a local address, check definition
		void addComponent(StrongEntityComponentPtr pComponent);
		void removeComponent(const char* componentName);

		void addChild(StrongEntityPtr child);
		void removeChild(EntityId id);
		bool hasChild() { return !m_childs.empty(); }
		bool hasParent() {
			if (m_parent != nullptr)
				return true;
			return false;
		}
		Entity* getParent() { return m_parent; }
		const std::vector<StrongEntityPtr> getChildList() { return m_childs; }
		StrongEntityPtr getChildWithID(const EntityId id);

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