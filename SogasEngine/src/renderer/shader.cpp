#include "shader.h"
#include "../external/GLEW/glew-2.1.0/include/GL/glew.h"

#include <iostream>
#include <fstream>
#include <sstream>

Shader::Shader(const std::string& filepath)
	: m_filePath(filepath), m_ID(0)
{
	ShaderProgramSource source = parseShader(filepath);
	m_ID = createShader(source.vertexSource, source.fragmentSource);
}

Shader::~Shader()
{
	glDeleteProgram(m_ID);
}

void Shader::bind() const
{
	glUseProgram(m_ID);
}

void Shader::unbind() const
{
	glUseProgram(0);
}

ShaderProgramSource Shader::parseShader(const std::string& filepath)
{
	std::ifstream stream(filepath);

	enum class ShaderType
	{
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};

	std::string line;
	std::stringstream ss[2];
	ShaderType type = ShaderType::NONE;
	while(getline(stream, line))
	{
		if(line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
				type = ShaderType::VERTEX;
			else if (line.find("fragment") != std::string::npos)
				type = ShaderType::FRAGMENT;
		}
		else
		{
			ss[(i32)type] << line << '\n';
		}

		return { ss[0].str(), ss[1].str() };
	}

	return ShaderProgramSource();
}

bool Shader::compileShader(u32 type, const std::string& source)
{
	u32 id = glCreateShader(type);
	// TODO: Solve the char to i8 issue
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	i32 result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if(result == GL_FALSE)
	{
		i32 length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)alloca(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);
		glDeleteShader(id);
		return 0;
	}

	return id;
}

u32 Shader::createShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	u32 program = glCreateProgram();
	u32 vs = compileShader(GL_VERTEX_SHADER, vertexShader);
	u32 fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);

	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}

u32 Shader::getUniformLocation(const std::string& name)
{
	if (m_uniformLocationCache.find(name) != m_uniformLocationCache.end())
		return m_uniformLocationCache[name];

	i32 location = glGetUniformLocation(m_ID, name.c_str());
	if (location == -1)
		std::cout << "Warning: uniform '" << name << "' doesn't exist!" << std::endl;
	
	m_uniformLocationCache[name] = location;
	return location;
}
