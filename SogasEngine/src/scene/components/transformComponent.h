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
		glm::mat4 m_inverseGlobalMatrix;

	private:
		glm::vec3 m_translation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 m_rotation	= { 0.0f, 0.0f, 0.0f };
		glm::vec3 m_scale		= { 1.0f, 1.0f, 1.0f };
		glm::mat4 m_localMatrix;	// local matrix, relative to the parent if any.
		glm::mat4 m_globalMatrix;	// world matrix, where the object is defined in the world.
		bool	  m_isLocal{true};
	public:
		static const char* s_name;
		virtual const char* getName() const override { return s_name; }

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;

		virtual bool				init() override;
		virtual void				to_json(json& j) override;
		virtual void				from_json(const json& j) override;
		virtual LuaPlus::LuaObject	toLuaObject(LuaPlus::LuaObject self) const override;
		virtual void				fromLuaObject(LuaPlus::LuaObject self) override;

		// Transform component functions
		glm::mat4	getGlobalTransform();
		glm::mat4	getLocalTransform();

		glm::vec3&	getTranslation() { return m_translation; }
		glm::vec3&	getRotation() { return m_rotation; }
		glm::vec3&	getScale() { return m_scale; }
		bool		getIsLocal() { return m_isLocal; }

		void setTransform(const glm::mat4& transform);
		void setTranslation(glm::vec3& translation) { m_translation = translation; updateMatrix(); }
		void setRotation(glm::vec3& rotation) { m_rotation = rotation; updateMatrix(); }
		void setScale(glm::vec3& scale) { m_scale = scale; updateMatrix(); }
		void setIsLocal(const bool local) { m_isLocal = local; }

		// TODO: getLookAt(void)

	private:
		void updateMatrix() {
			m_localMatrix = glm::translate(glm::mat4(1), m_translation) *
				glm::mat4_cast(glm::quat(m_rotation)) *
				glm::scale(glm::mat4(1), m_scale);
		}
	};
}