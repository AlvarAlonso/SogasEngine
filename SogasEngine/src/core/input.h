#pragma once

#include "defines.h"
#include "../external/glm/glm/glm.hpp"

namespace Sogas 
{
	class SGS Input
	{
	public:
		// Keys
		static bool isKeyPressed(i32 keycode);

		// Mouse
		static bool isMouseButtonPressed(i32 button);
		static bool isMouseButtonReleased(i32 button);
		static glm::vec2 getMousePosition();
		static void setMousePosition(f32 x, f32 y);
		static void centerMouse();
	};
}