#pragma once

#include "entityComponent.h"
#include "glm/glm.hpp"

namespace Sogas
{
	/*
	* The Light Component class.
	* 
	* Features:
	*	- Three light types: ENVIRONMENT, POINT and SPOT.
	*	- Each type with its own unique parameters to be modified in editor runtime.
	*	- In SPOT type, the direction of the light cast is being controlled by the 
	*		transform. Transform rotation determines where it is facing.
	*/
	class SGS LightComponent : public EntityComponent
	{
	public:

		static const char* s_name;

		enum class e_lightType { ENVIRONMENT = 0, POINT, SPOT };

		LightComponent() = default;
		~LightComponent() = default;

		virtual bool init() override;
		virtual const char* getName() const override { return s_name; }
		virtual void to_json(json& j) override;
		virtual void from_json(const json& j) override;
		virtual LuaPlus::LuaObject toLuaObject(LuaPlus::LuaObject self) const override { return LuaPlus::LuaObject(); };
		virtual void fromLuaObject(LuaPlus::LuaObject) override {};

		inline glm::vec3& getColor() { return m_color; }
		inline f32& getMaxDistance() { return m_maxDistance; }
		inline f32& getIntensity() { return m_intensity; }
		inline e_lightType getType() { return m_type; }
		inline f32& getAngle() { return m_angle; }

		void setColor(const glm::vec3 color) { m_color = color; }
		void setType(const e_lightType type) { m_type = type; }

	private:
		e_lightType m_type{ e_lightType::POINT };
		glm::vec3	m_color{ 1 };
		glm::vec3	m_direction{ 0.0f, 0.0f, 1.0f };
		f32			m_maxDistance{ 100 };
		f32			m_intensity{ 1 };
		f32			m_angle{ 0.0f };
	};
}