#pragma once

#include "defines.h"

#include <glm/glm.hpp>
#include <string>

namespace Sogas 
{
	class SGS Shader
	{
	public:
		static std::unordered_map<std::string, std::shared_ptr<Shader>> s_loadedShaders;

		virtual ~Shader() = default;

		virtual void bind() const = 0;
		virtual void unbind() const = 0;
		virtual std::string getName() const = 0;

		virtual void setUniform(const char* varname, bool input) = 0;
		virtual void setUniform(const char* varname, int input) = 0;
		virtual void setUniform(const char* varname, float input) = 0;
		virtual void setUniform(const char* varname, glm::vec3 input) = 0;
		virtual void setUniform(const char* varname, glm::vec4 input) = 0;
		virtual void setUniform(const char* varname, glm::mat4 input) = 0; 

		static std::shared_ptr<Shader> create(const std::string& filepath);
		static std::shared_ptr<Shader> create(const std::string& vertexSource, const std::string& fragmentSource);

		static std::shared_ptr<Shader> GET(const std::string filepath);
		static std::shared_ptr<Shader> GETDefault(std::string name);

	};
}