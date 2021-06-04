#pragma once

#include "defines.h"
#include "../external/glm/glm/glm.hpp"

class SGS Camera
{
public:
	Camera(glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f), f32 fov = 60.0f)
		: m_position(pos), m_fov(fov), m_direction(glm::vec3(0.0f, 0.0f, 1.0f)), m_up(glm::vec3(0, 1, 0)), m_right(glm::vec3(1, 0, 0))
	{}

	glm::mat4 getView();
	glm::mat4 getProjection();
	glm::mat4 getViewProjection();

	inline void setPosition(glm::vec3 pos) { m_position = pos; }
	inline glm::vec3 getPosition() const { return m_position; }

private:
	glm::vec3 m_position;
	glm::vec3 m_direction;
	glm::vec3 m_up;
	glm::vec3 m_right;

	// TODO: to rotate the camera
	f32 m_yaw = 90.0f;
	f32 m_pitch = 0.0f;
	f32 m_speed = 0.1f;
	f32 m_fov;
};
