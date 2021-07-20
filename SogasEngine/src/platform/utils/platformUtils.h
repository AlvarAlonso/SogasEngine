#pragma once

#include "defines.h"
#include <string>

namespace Sogas
{
	class SGS FileDialog
	{
	public:
		inline static std::string openFile(const char* filter);
		inline static std::string saveFile(const char* filter);
	};
}