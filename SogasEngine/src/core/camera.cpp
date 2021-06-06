#include "camera.h"

#include "application.h"
#include "glm/glm/gtc/matrix_transform.hpp"

glm::mat4 Camera::getView()
{
    return glm::lookAt(m_position, m_position + m_direction, m_up);
}

glm::mat4 Camera::getProjection()
{
    f32 aspect = (f32)Application::getInstance()->getWindow().getWidth() / (f32)Application::getInstance()->getWindow().getHeight();
    return glm::perspective(glm::radians(m_fov), aspect, 0.001f, 1000.0f);
}

glm::mat4 Camera::getViewProjection()
{
    return getProjection() * getView();
}

void Camera::rotate(f32 xoffset, f32 yoffset, bool constrainPitch)
{
    xoffset *= m_sensivity;
    yoffset *= m_sensivity;

    m_yaw -= xoffset;
    m_pitch += yoffset;

    if (constrainPitch){
        if (m_pitch > 89.0f)
            m_pitch = 89.0f;
        if (m_pitch < -89.0f)
            m_pitch = -89.0f;
    }

    glm::vec3 front;
    front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    front.y = sin(glm::radians(m_pitch));
    front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));

    m_direction = glm::normalize(front);
    m_right     = glm::normalize(glm::cross(m_direction, glm::vec3(0, 1, 0)));
    m_up        = glm::normalize(glm::cross(m_right, m_direction));
}

void Camera::move(e_cameraMovement dir, f32 dt)
{
    switch (dir)
    {
    case FORWARD:
        setPosition(m_position + m_direction * dt);
        break;
    case BACKWARD:
        setPosition(m_position - m_direction * dt);
        break;
    case LEFT:
        setPosition(m_position - m_right * dt);
        break;
    case RIGHT:
        setPosition(m_position + m_right * dt);
        break;
    case UP:
        break;
    case DOWN:
        break;
    default:
        break;
    }
}