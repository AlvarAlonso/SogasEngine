#pragma once

#include "entityComponent.h"
#include "scripting/EntityScript.h"

namespace Sogas
{
	class SGS EntityScriptComponent : public EntityComponent
	{
	public:
		static const char* s_name;

		EntityScriptComponent() = default;
		~EntityScriptComponent() = default;

		virtual bool init() override;
		virtual const char* getName() const override { return s_name; }
		virtual void to_json(json& j) override;
		virtual void from_json(const json& j) override;
		virtual void update(f32 dt) override { if(m_entityScript) m_entityScript->update(); }

		void setEntityScript(EntityScript* entityScript) { m_entityScript = entityScript; }

	private:
		bool m_started = false;
		EntityScript* m_entityScript = nullptr;
	};
}

