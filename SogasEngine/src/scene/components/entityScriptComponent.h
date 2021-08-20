#pragma once

#include "entityComponent.h"
#include "scripting/EntityScript.h"
#include "core/assertions.h"

namespace Sogas
{
	class SGS EntityScriptComponent : public EntityComponent
	{
	public:
		static const char* s_name;

		EntityScriptComponent() = default;
		~EntityScriptComponent() = default;

		virtual bool init() override;
		virtual void postInit(void) override;
		virtual const char* getName() const override { return s_name; }
		virtual void to_json(json& j) override;
		virtual void from_json(const json& j) override;
		virtual LuaPlus::LuaObject toLuaObject(LuaPlus::LuaObject self) const override { return LuaPlus::LuaObject(); };
		virtual void fromLuaObject(LuaPlus::LuaObject) const override {};

		virtual void update(f32 dt) override { if(m_entityScript) m_entityScript->update(); }

		ScriptVariablesMap getEntityScriptVariables() const { SGSASSERT(m_entityScript); return m_entityScript->getScriptVariables(); }

		// TODO: find another way to initialize the script, for example, just passing the name of the class and the engine itself
		// creates an instance of the script. The only one dealing with the script should be the component, and it should not
		// include interface functions who deal with the entityScript.
		void setEntityScript(EntityScript* entityScript) { m_entityScript = entityScript; }

	private:
		bool m_started = false;
		EntityScript* m_entityScript = nullptr;
	};
}

