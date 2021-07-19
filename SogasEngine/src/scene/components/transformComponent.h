#pragma once

#include "defines.h"

#include "entityComponent.h"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"

namespace Sogas
{
	class SGS TransformComponent : public EntityComponent
	{
	public:
		static const char* s_name;
		virtual const char* getName() const override { return s_name; }

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::vec3& translation) : m_translation(translation){}
		//TransformComponent(void) : m_transform(glm::mat4(1)) {};

		virtual bool init() override;
		virtual void to_json(json& j) override;

		// Transform component functions		
		glm::mat4 getTransform() const
		{
			return glm::translate(glm::mat4(1.0f), m_translation)
				* glm::mat4_cast(glm::quat(m_rotation))
				* glm::scale(glm::mat4(1.0f), m_scale);
		}

		void setTransform(const glm::mat4& transform)
		{
			m_translation = (glm::vec3)transform[0][3];
			m_rotation = glm::eulerAngles(glm::quat_cast(transform));
			m_scale = glm::vec3(transform[0][0], transform[1][1], transform[2][2]);
		}

		glm::vec3& getTranslation() { return m_translation; }
		glm::vec3& getRotation() { return m_rotation; }
		glm::vec3& getScale() { return m_scale; }

		void setTranslation(glm::vec3& translation) { m_translation = translation; }
		void setRotation(glm::vec3& rotation) { m_rotation = rotation; }
		void setScale(glm::vec3& scale) { m_scale = scale; }

		// TODO: getLookAt(void)

	private:
		glm::vec3 m_translation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 m_rotation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 m_scale = { 1.0f, 1.0f, 1.0f };
	};
}