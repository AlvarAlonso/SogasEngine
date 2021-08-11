#pragma once

#include "defines.h"
#include "logger.h"
#include "assertions.h"

#include <string>
#include <vector>
#include <fstream>

namespace Sogas
{
	SGS std::string findFile(const std::string& filename, const std::vector<std::string>& directories);
	SGS std::string takeNameFromPath(const std::string& path);
}