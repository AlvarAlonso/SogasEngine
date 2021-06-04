#include "core/input.h"

#include "core/application.h"

bool Input::isKeyPressed(i32 keycode)
{
	auto* window = static_cast<GLFWwindow*>(Application::getInstance()->getWindow().getNativeWindow());
	auto state = glfwGetKey(window, keycode);
	return state == GLFW_PRESS || state == GLFW_REPEAT;
}

bool Input::isMouseButtonPressed(i32 button)
{
	GLFWwindow* window = Application::getInstance()->getWindow().getNativeWindow();
	auto state = glfwGetMouseButton(window, button);
	return state == GLFW_PRESS;
}

glm::vec2 Input::getMousePosition()
{
	GLFWwindow* window = Application::getInstance()->getWindow().getNativeWindow();
	f64 xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);
	return glm::vec2(xpos, ypos);
}

