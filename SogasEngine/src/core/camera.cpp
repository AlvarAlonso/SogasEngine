#include "sgspch.h"

#include "camera.h"

#include "application.h"
#include "glm/glm/gtc/matrix_transform.hpp"

namespace Sogas 
{
    Camera::Camera(glm::vec3 pos, f32 fov, f32 aspectRatio)
        : m_position(pos), m_fov(fov), m_front(glm::vec3(0.0f, 0.0f, 1.0f)), m_up(glm::vec3(0, 1, 0)), m_right(glm::vec3(1, 0, 0)), m_aspectRatio(aspectRatio)
    {

    }

    glm::mat4 Camera::getView()
    {
        return glm::lookAt(m_position, m_position + m_front, m_up);
    }

    glm::mat4 Camera::getProjection()
    {
        if(m_fixedAspectRatio)
        {
            return glm::perspective(glm::radians(m_fov), m_aspectRatio, 0.001f, 1000.0f);
        }
        else
        {
            return glm::perspective(glm::radians(m_fov), m_aspectRatio, 0.001f, 1000.0f);
        }
    }

    glm::mat4 Camera::getViewProjection()
    {
        return getProjection() * getView();
    }

    void Camera::rotate(f32 xoffset, f32 yoffset, bool constrainPitch)
    {
        m_yaw -= xoffset * m_orbitSpeed;
        m_pitch += yoffset * m_orbitSpeed;

        if (constrainPitch) {
            if (m_pitch > 89.0f)
                m_pitch = 89.0f;
            if (m_pitch < -89.0f)
                m_pitch = -89.0f;
        }

        glm::vec3 front;
        front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
        front.y = sin(glm::radians(m_pitch));
        front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));

        m_front = glm::normalize(front);
        m_right = glm::normalize(glm::cross(glm::vec3(0, 1, 0), m_front));
        m_up = glm::normalize(glm::cross(m_front, m_right));
    }

    void Camera::orbit(f32 xoffset, f32 yoffset)
    {
        glm::vec3 movement = m_right * xoffset + m_up * yoffset;
        glm::vec3 newPosition = m_position + movement;
        setPosition(newPosition);
        m_front += movement;
    }

    void Camera::move(e_cameraMovement dir, f32 dt)
    {
        switch (dir)
        {
        case FORWARD:
            setPosition(m_position + m_front * dt);
            break;
        case BACKWARD:
            setPosition(m_position - m_front * dt);
            break;
        case LEFT:
            setPosition(m_position + m_right * dt);
            break;
        case RIGHT:
            setPosition(m_position - m_right * dt);
            break;
        case UP:
            setPosition(m_position + m_up * dt);
            break;
        case DOWN:
            setPosition(m_position - m_up * dt);
            break;
        default:
            break;
        }
    }

    void Camera::pan(f32 xoffset, f32 yoffset)
    {
        glm::vec3 movement = m_right * xoffset + m_up * yoffset;
        glm::vec3 newPosition = m_position + movement;
        setPosition(newPosition);
    }

    void Camera::zoom(const f32 factor)
    {
        glm::vec3 zoom = m_front * factor * m_orbitSpeed;
        m_position += zoom;
        m_front += zoom;
    }

    // TODO function not working
    void Camera::lookat(const glm::vec3 position, const glm::vec3 center)
    {
        glm::mat4 transform = glm::lookAt(position, position + center, m_up);

        m_position = position;
        m_front = glm::normalize(glm::vec3(transform[2]));
        m_right = glm::cross(m_front, glm::vec3(0, 1, 0));
        //m_rotation = glm::eulerAngles(glm::quat_cast(transform));
    }
}