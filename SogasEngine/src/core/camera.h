#pragma once

#include "defines.h"
#include "../external/glm/glm/glm.hpp"

enum e_cameraMovement
{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	UP,
	DOWN
};

class SGS Camera
{
public:
	Camera(glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f), f32 fov = 45.0f)
		: m_position(pos), m_fov(fov), m_direction(glm::vec3(0.0f, 0.0f, 1.0f)), m_up(glm::vec3(0, 1, 0)), m_right(glm::vec3(1, 0, 0))
	{}

	glm::mat4 getView();
	glm::mat4 getProjection();
	glm::mat4 getViewProjection();

	void rotate(f32 xoffset, f32 yoffset, bool constrainPitch = true);
	void move(e_cameraMovement dir, f32 dt);
	void update(i32 keycode);

	inline void setPosition(glm::vec3 pos) { m_position = pos; }
	inline glm::vec3 getPosition() const { return m_position; }
	inline glm::vec3 getDirection() const { return m_direction; }
	inline glm::vec3 getRight() const { return m_right; }

	bool m_locked = false;

private:
	glm::vec3 m_position;
	glm::vec3 m_direction;
	glm::vec3 m_up;
	glm::vec3 m_right;

	// TODO: to rotate the camera
	f32 m_yaw = 90.0f;
	f32 m_pitch = 0.0f;
	f32 m_speed = 0.1f;
	f32 m_sensivity = 0.1f;
	f32 m_fov;
};
