#pragma once

#include "entityComponent.h"
#include "glm/glm.hpp"

namespace Sogas
{
	class SGS LightComponent : public EntityComponent
	{
	public:

		static const char* s_name;

		LightComponent() = default;
		~LightComponent() = default;

		virtual bool init() override;
		virtual const char* getName() const override { return s_name; }
		virtual void to_json(json& j) override;
		virtual void from_json(const json& j) override;
		virtual LuaPlus::LuaObject toLuaObject(LuaPlus::LuaObject self) const override { return LuaPlus::LuaObject(); };
		virtual void fromLuaObject(LuaPlus::LuaObject) const override {};

		inline glm::vec3& getColor() { return m_color; }
		inline f32& getMaxDistance() { return m_maxDistance; }
		inline f32& getIntensity() { return m_intensity; }

		void setColor(const glm::vec3 color) { m_color = color; }

	private:
		glm::vec3 m_color{ 1 };
		f32 m_maxDistance{ 100 };
		f32 m_intensity{ 1 };
	};
}