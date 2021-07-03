#pragma once

#include "defines.h"

#include "types.h"
#include <unordered_map>

namespace Sogas
{

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
		std::shared_ptr<Entity> m_pOwner;

	public:
		virtual ~EntityComponent() { m_pOwner.reset(); }

		// Functions meant to be overriden by the implementation class of the components
		virtual bool init() = 0;
		virtual void postInit(void) {}
		virtual void update(f32 dt) {}
		virtual void onChanged(void) {}

		// Functions meant to be overriden by the interface class
		virtual ComponentId getId() const { return getIdFromName(getName()); }
		virtual const char* getName() const = 0;

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
		void setOwner(StrongEntityPtr pOwner) { m_pOwner = pOwner; }
		static std::unordered_map<std::string, u32> m_componentTypes;
		static u32 lastId;
	};
}