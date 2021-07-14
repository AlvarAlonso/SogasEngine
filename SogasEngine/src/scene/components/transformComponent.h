#pragma once

#include "defines.h"

#include "entityComponent.h"
#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"

//#include <nlohmann/json.hpp>

namespace Sogas
{

	//using json = nlohmann::json;

	class SGS TransformComponent : public EntityComponent
	{
	public:
		static const char* s_name;
		virtual const char* getName() const override { return s_name; }

		TransformComponent(void) : m_transform(glm::mat4(1)) {};
		virtual bool init(void) override;

		// Transform component functions
		glm::mat4& getTransform(void) { return m_transform; }
		void setTransform(glm::mat4& transform) { m_transform = transform; }

		glm::vec3 getTranslation(void) { return glm::vec3(m_transform[0][3]); }
		glm::quat getRotation(void) { return glm::quat_cast(m_transform); }
		glm::vec3 getScale(void) { return glm::vec3(m_transform[0][0], m_transform[1][1], m_transform[2][2]); }

		//static void to_json(json& j, const std::shared_ptr<TransformComponent>& transformComponent);

		//glm::vec3 getPosition(void) {}
		// TODO: setPosition(glm::vec3 position)
		// TODO: getLookAt(void)

	private:
		glm::mat4 m_transform;
	};
}