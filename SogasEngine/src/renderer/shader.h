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

		static Shader* create(const std::string& vertexSource, const std::string& fragmentSource);
		static Shader* create(const std::string& filepath);
	};
}