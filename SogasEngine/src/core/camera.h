#pragma once

#include "defines.h"
#include "../external/glm/glm/glm.hpp"

namespace Sogas 
{
	enum e_cameraMovement
	{
		FORWARD,
		BACKWARD,
		LEFT,
		RIGHT,
		UP,
		DOWN
	};


	// TODO: rethink using components
	class SGS Camera
	{
	public:
		Camera(glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f), f32 fov = 45.0f, f32 aspectRatio = 4.0f / 3.0f);

		glm::mat4 getView();
		glm::mat4 getProjection();
		glm::mat4 getViewProjection();

		void rotate(f32 xoffset, f32 yoffset, bool constrainPitch = true);
		void orbit(f32 xoffset, f32 yoffset);
		void move(e_cameraMovement dir, f32 dt);
		void pan(f32 xoffset, f32 yoffset);
		void update(i32 keycode);
		void zoom(const f32 factor);
		void lookat(const glm::vec3 position, const glm::vec3 center);

		inline void setPosition(glm::vec3 pos) { m_position = pos; }
		inline void setViewportSize(const f32 width, const f32 height) { m_viewportSize.x = width, m_viewportSize.y = height; m_aspectRatio = width / height; }
		inline glm::vec3 getPosition() const { return m_position; }
		inline glm::vec3 getDirection() const { return m_front; }
		inline glm::vec3 getRight() const { return m_right; }
		inline f32 getAspectRatio() const { return m_aspectRatio; }

		bool m_locked = false;
		bool m_fixedAspectRatio = false;

	private:

		f32 m_aspectRatio;

		glm::vec2 m_viewportSize;

		glm::vec3 m_position;
		glm::vec3 m_front;
		glm::vec3 m_up;
		glm::vec3 m_right;

		f32 m_orbitSpeed = 5.0f;

		// TODO: to rotate the camera
		f32 m_yaw = 90.0f;
		f32 m_pitch = 0.0f;
		f32 m_fov;
	};
}