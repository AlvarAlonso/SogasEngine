#include "utils.h"
#include "sgspch.h"

namespace Sogas 
{
	/*
	* @brief Function to find the path given the name of file and a list of directories to search for
	* @param const std::string&
	* @param const std::vector<std::string>&
	* @return std::string
	*/
	std::string findFile(const std::string& filename, const std::vector<std::string>& directories)
	{
		std::ifstream stream;

		{
			stream.open(filename.c_str());
			if (stream.is_open())
				return filename;
		}

		for (const auto& directory : directories)
		{
			std::string file = directory + "/" + filename;
			stream.open(file);
			if (stream.is_open())
				return file;
		}

		SGSWARN("File not found %s in directories:\n", filename.c_str());
		for (const auto& dir : directories)
		{
			SGSWARN("%s\n", dir.c_str());
		}
	}

	/*
	* @brief This function takes a path and selects only the name of the file.
	* Goes till the last '/' and '.' and returns "name.extension"
	* @param const std::string&
	* @return std::string
	*/
	std::string takeNameFromPath(const std::string& path)
	{
		auto lastSlash = path.find_last_of("/\\");
		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
		auto end = path.size();
		auto count = end == std::string::npos ? path.size() - lastSlash : end - lastSlash;
		std::string name = path.substr(lastSlash, count);
		return name;
	}
}