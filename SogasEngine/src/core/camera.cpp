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
    return glm::perspective(m_fov, aspect, 0.001f, 1000.0f);
}

glm::mat4 Camera::getViewProjection()
{
    return getProjection() * getView();
}