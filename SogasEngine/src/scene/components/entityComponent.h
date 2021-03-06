#pragma once

#include "defines.h"

#include "../types.h"
#include <unordered_map>
#include <json/single_include/nlohmann/json.hpp>

#include "scripting/LuaStateManager.h"

namespace Sogas
{
	// TODO: use these name constants instead of the hardcoded string
	// TODO: moved the named constants to another file due to linking errors
	// component static names
	/*
	const char* RENDER_COMPONENT_NAME = "RenderComponent";
	const char* LIGHT_COMPONENT_NAME = "LightComponent";
	const char* CAMERA_COMPONENT_NAME = "CameraComponent";
	const char* ENTITY_SCRIPT_COMPONENT_NAME = "EntityScriptComponent";
	const char* TRANSFORM_COMPONENT_NAME = "TransformComponent";
	*/

	using json = nlohmann::json;

	//TODO: find a better way to find a const char* in a map
	struct cmp_str
	{
		bool operator()(char const* a, char const* b) const
		{
			return strcmp(a, b) < 0;
		}
	};

	class SGS EntityComponent
	{
	protected:
		WeakEntityPtr m_pOwner;

	public:
		virtual ~EntityComponent() { m_pOwner.reset(); }

		// Functions meant to be overriden by the implementation class of the components
		virtual bool init() = 0;
		virtual void postInit(void) {}
		virtual void update(f32 dt) {}
		virtual void onChanged(void) {}

		// Functions meant to be overriden by the interface class
		virtual ComponentId getId() const { return getIdFromName(getName()); } // This function returns the component ID not the Entity ID!!!
		virtual const char* getName() const = 0;
		virtual void		to_json(json& j) = 0;
		virtual void		from_json(const json& j) = 0;
		virtual LuaPlus::LuaObject toLuaObject(LuaPlus::LuaObject self) const = 0;
		virtual void fromLuaObject(LuaPlus::LuaObject) = 0;

		void setOwner(WeakEntityPtr pOwner) { m_pOwner = pOwner; }

		WeakEntityPtr getOwner(void) { return m_pOwner; }

		// TODO: Should probably use hash instead of map for performance sake or maybe bitwise
		// It fails in the find func with const char*
		static ComponentId getIdFromName(const char* componentStr)
		{
			std::string s = componentStr;
			std::unordered_map<std::string, u32>::iterator it = m_componentTypes.find(s);
			if (it == m_componentTypes.end())
			{
				m_componentTypes.insert(std::make_pair(s, lastId));
				lastId++;
				return static_cast<ComponentId>(lastId - 1);
			}
			else {
				return static_cast<ComponentId>(it->second);
			}
		}

	private:
		static std::unordered_map<std::string, u32> m_componentTypes;
		static u32 lastId;
	};
}