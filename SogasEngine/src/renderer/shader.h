#pragma once

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
	u32 m_ID;
	std::unordered_map<std::string, i32> m_uniformLocationCache;
public:
	Shader(const std::string& filepath);
	~Shader();

	void bind() const;
	void unbind() const;

private:
	ShaderProgramSource parseShader(const std::string& filepath);
	bool compileShader(u32 type, const std::string& source);
	u32 createShader(const std::string& vertexShader, const std::string& fragmentShader);
	u32 getUniformLocation(const std::string& name);
};