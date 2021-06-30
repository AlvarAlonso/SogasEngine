#pragma once

#include "entityComponent.h"
#include "../external/glm/glm/glm.hpp"

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

		inline glm::vec3& getColor() { return m_color; }
		inline f32 getMaxDistance() { return m_maxDistance; }
		inline f32 getIntensity() { return m_intensity; }

		void setColor(const glm::vec3 color) { m_color = color; }

	private:
		glm::vec3 m_color{ 0 };
		f32 m_maxDistance{ 0 };
		f32 m_intensity{ 0 };
	};
}