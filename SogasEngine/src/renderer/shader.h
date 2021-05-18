#pragma once

#include "../external/GLEW/glew-2.1.0/include/GL/glew.h"
#include "defines.h"
#include <string>
#include <unordered_map>

struct ShaderProgramSource
{
	std::string vertexSource;
	std::string fragmentSource;
};

class Shader
{
private:
	std::string m_filePath;
	std::unordered_map<std::string, i32> m_uniformLocationCache;
public:
	u32 m_ID;

	Shader(const std::string& filepath);
	~Shader();

	void bind() const;
	void unbind() const;

	// upload
	void setUniform(const char* varname, bool input) { setUniform1(varname, input); }
	void setUniform(const char* varname, int input) { setUniform1(varname, input); }
	void setUniform(const char* varname, float input) { setUniform1(varname, input); }

	virtual void setUniform1(const char* varname, const bool input1);
	virtual void setUniform1(const char* varname, const int input1);
	virtual void setUniform1(const char* varname, const float input);

private:
	ShaderProgramSource parseShader(const std::string& filepath);
	u32 compileShader(u32 type, const std::string& source);
	u32 createShader(const std::string& vertexShader, const std::string& fragmentShader);
	u32 getUniformLocation(const std::string& name);
};