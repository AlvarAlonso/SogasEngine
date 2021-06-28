#pragma once

#include "defines.h"

#include "entityComponent.h"

#include "../external/glm/glm/glm.hpp"

namespace Sogas
{
	class SGS TransformComponent : public EntityComponent
	{
	public:
		static const char* s_name;
		virtual const char* getName() const override { return s_name; }

		TransformComponent(void) : m_transform(glm::mat4(1)) {};
		virtual bool init() override;

		// Transform component functions
		glm::mat4 getTransform(void) const { return m_transform; }
		void setTransform(glm::mat4& transform) { m_transform = transform; }
		//glm::vec3 getPosition(void) {}
		// TODO: setPosition(glm::vec3 position)
		// TODO: getLookAt(void)
	private:
		glm::mat4 m_transform;
	};
}