#pragma once

#include "defines.h"
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

		static std::shared_ptr<Shader> create(const std::string& vertexSource, const std::string& fragmentSource);
		static std::shared_ptr<Shader> create(const std::string& filepath);

		static std::shared_ptr<Shader> GET(const std::string filepath);

	};
}