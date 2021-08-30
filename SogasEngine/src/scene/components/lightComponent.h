#pragma once

#include "entityComponent.h"
#include "renderer/resources/light.h"
#include "glm/glm.hpp"

namespace Sogas
{
	class SGS LightComponent : public EntityComponent
	{
	public:

		static const char* s_name;

		LightComponent();
		~LightComponent() = default;

		virtual bool init() override;
		virtual const char* getName() const override { return s_name; }
		virtual void to_json(json& j) override;
		virtual void from_json(const json& j) override;
		virtual LuaPlus::LuaObject toLuaObject(LuaPlus::LuaObject self) const override { return LuaPlus::LuaObject(); };
		virtual void fromLuaObject(LuaPlus::LuaObject) override {};

		std::weak_ptr<Light> getLight() const { return m_light; }
		inline glm::vec3& getColor() { return m_light->m_color; }
		inline f32& getMaxDistance() { return m_light->m_maxDistance; }
		inline f32& getIntensity() { return m_light->m_intensity; }

		void setColor(const glm::vec3 color) { m_light->m_color = color; }

	private:
		std::shared_ptr<Light> m_light;
	};
}