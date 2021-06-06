#pragma once

#include "defines.h"
//#include "glm/glm.hpp"
#include "../external/glm/glm/glm.hpp"

class SGS Input
{
public:
	// Keys
	static bool isKeyPressed(i32 keycode);

	// Mouse
	static bool isMouseButtonPressed(i32 button);
	static bool isMouseButtonReleased(i32 button);
	static glm::vec2 getMousePosition();
};