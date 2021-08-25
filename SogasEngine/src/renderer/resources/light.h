#pragma once

#include "defines.h"
#include <glm/vec3.hpp>

namespace Sogas
{
	class Light
	{
	public:
		glm::vec3 m_color{ 1 };
		f32 m_maxDistance{ 100 };
		f32 m_intensity{ 1 };
	};
}