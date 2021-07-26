#pragma once

#include "core/assertions.h"
#include <string>
#include <unordered_map>

#include "../external/GLEW/glew-2.1.0/include/GL/glew.h"
#include "renderer/shader.h"
//#include "../external/glm/glm/glm.hpp"

namespace Sogas 
{
	struct ShaderProgramSource
	{
		std::string vertexSource;
		std::string fragmentSource;
	};

	class OpenGLShader : public Shader
	{
	private:
		std::string m_filePath;
		std::unordered_map<std::string, i32> m_uniformLocationCache;
	public:
		u32 m_ID;

		OpenGLShader(const std::string& vertexSource, const std::string& fragmentSource);
		OpenGLShader(const std::string& filepath);
		~OpenGLShader();

		void bind() const;
		void unbind() const;
		virtual std::string getName() const { return m_filePath; }

		// upload
		virtual void setUniform(const char* varname, bool input)		override { setUniform1(varname, input); }
		virtual void setUniform(const char* varname, int input)			override { setUniform1(varname, input); }
		virtual void setUniform(const char* varname, float input)		override { setUniform1(varname, input); }
		virtual void setUniform(const char* varname, glm::vec3 input)	override { setUniform3(varname, input); }
		virtual void setUniform(const char* varname, glm::vec4 input)	override { setUniform4(varname, input); }
		virtual void setUniform(const char* varname, glm::mat4 input)	override { setMatrix44(varname, input); }

		virtual void setUniform1(const char* varname, const bool input1);
		virtual void setUniform1(const char* varname, const int input1);
		virtual void setUniform1(const char* varname, const float input);
		virtual void setUniform3(const char* varname, const glm::vec3 input);
		virtual void setUniform4(const char* varname, const glm::vec4 input);
		virtual void setMatrix44(const char* varname, const glm::mat4 input);

	private:
		ShaderProgramSource parseShader(const std::string& filepath);
		u32 compileShader(u32 type, const std::string& source);
		u32 createShader(const std::string& vertexShader, const std::string& fragmentShader);
		u32 getUniformLocation(const std::string& name);
	};
}