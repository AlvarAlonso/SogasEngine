#pragma once

#include "defines.h"
#include <string>

namespace Sogas 
{
	class SGS Shader
	{
	public:
		virtual ~Shader() = default;

		virtual void bind() const = 0;
		virtual void unbind() const = 0;

		static std::shared_ptr<Shader> create(const std::string& vertexSource, const std::string& fragmentSource);
		static std::shared_ptr<Shader> create(const std::string& filepath);
	};
}