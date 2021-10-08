#pragma once

#include "entityComponent.h"
#include "core/camera.h"

namespace Sogas
{
	class SGS CameraComponent : public EntityComponent
	{
	public:
		static const char* s_name;

		CameraComponent() = default;

		enum class eProjectionType { Perspective = 0, Orthographic = 1};

		virtual const char* getName() const override { return s_name; }
		virtual bool		init() override;
		virtual void		postInit() override;

		virtual void		to_json(json& j) override;
		virtual void		from_json(const json& j) override;
		virtual LuaPlus::LuaObject toLuaObject(LuaPlus::LuaObject self) const override { return LuaPlus::LuaObject(); };
		virtual void fromLuaObject(LuaPlus::LuaObject) override {};

		const glm::mat4& getProjection(void) { return m_projection; }
		eProjectionType getType(void) const { return m_projectionType; }
		bool isPrimary() { return m_primary; }
		void setPrimary(const bool state) { m_primary = state; }
		bool getShowFrustrum() { return m_showFrustrum; }
		void setShowFrustrum(const bool state) { m_showFrustrum = state; }
		f32 getAspectRatio() const { return m_aspectRatio; }

		void setPerspective(f32 fov, f32 near, f32 far);
		void setOrtographic(f32 size, f32 near, f32 far);

		void setType(eProjectionType type) { m_projectionType = type; recalculateProjection(); }
		void setViewportSize(u32 width, u32 height);

		// Orthographic functions
		f32& getOrthographicSize() { return m_orthographicSize; }
		void setOrthographicSize(f32 size) { m_orthographicSize = size; recalculateProjection(); }
		f32& getOrthographicFar() { return m_orthographicFar; }
		void setOrthographicFar(f32 far) { m_orthographicFar = far; recalculateProjection(); }
		f32& getOrthographicNear() { return m_orthographicNear; }
		void setOrthographicNear(f32 near) { m_orthographicNear = near; recalculateProjection(); }

		// Perspective function
		f32 getPerspectiveFOV() { return m_perspectiveFOV; }
		void setPerspectiveFOV(f32 fov) { m_perspectiveFOV = fov; recalculateProjection(); }
		f32 getPerspectiveNear() { return m_perspectiveNear; }
		void setPerspectiveNear(f32 near) { m_perspectiveNear = near; recalculateProjection(); }
		f32 getPerspectiveFar() { return m_perspectiveFar; }
		void setPerspectiveFar(f32 far) { m_perspectiveFar = far; recalculateProjection(); }
	private:
		void recalculateProjection();

	private:

		eProjectionType m_projectionType = eProjectionType::Orthographic;

		glm::mat4 m_projection	= glm::mat4(1.0f);
		f32 m_aspectRatio		= 0.0f;

		f32 m_perspectiveFOV	= glm::radians(45.0f);	// FOV angle in radians.
		f32 m_perspectiveNear	= 0.01f;
		f32 m_perspectiveFar	= 1000.0f;

		f32 m_orthographicSize	= 1.0f;
		f32 m_orthographicNear	= 1.0f;
		f32 m_orthographicFar	= 100.0f;

		bool m_primary = false;
		bool m_showFrustrum = false;
	};
}