#pragma once

#include "defines.h"

#include "entityComponent.h"
#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"

#include "../external/glm/glm/glm.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include "../external/glm/glm/gtx/quaternion.hpp"

namespace Sogas
{

	//using json = nlohmann::json;

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

		// Transform component functions		
		glm::mat4 getTransform() const
		{
			glm::mat4 rotation = glm::toMat4(glm::quat(m_rotation));
			
			return glm::translate(glm::mat4(1.0f), m_translation)
				* rotation
				* glm::scale(glm::mat4(1.0f), m_scale);
		}

		void setTransform(const glm::mat4& transform)
		{
			m_translation = (glm::vec3)transform[0][3];
			//m_rotation = 
			m_scale = glm::vec3(transform[0][0], transform[1][1], transform[2][2]);
		}

		glm::vec3& getTranslation() { return m_translation; }
		glm::vec3& getRotation() { return m_rotation; }
		glm::vec3& getScale() { return m_scale; }

		void setTranslation(glm::vec3& translation) { m_translation = translation; }
		void setRotation(glm::vec3& rotation) { m_rotation = rotation; }
		void setScale(glm::vec3& scale) { m_scale = scale; }
		//glm::vec3 getPosition(void) {}
		// TODO: setPosition(glm::vec3 position)
		// TODO: getLookAt(void)

	private:
		glm::vec3 m_translation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 m_rotation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 m_scale = { 1.0f, 1.0f, 1.0f };
	};
}