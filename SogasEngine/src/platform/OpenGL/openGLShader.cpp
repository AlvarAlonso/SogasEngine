#include "openGLShader.h"
#include "renderer/shader.h"
#include "core/assertions.h"

#include "GL/glew.h"
#include <fstream>
#include <sstream>

#include "core/logger.h"

OpenGLShader::OpenGLShader(const std::string& vertexSource, const std::string& fragmentSource)
{
	m_ID = createShader(vertexSource, fragmentSource);
}

OpenGLShader::OpenGLShader(const std::string& filepath)
	: m_filePath(filepath), m_ID(0)
{
	ShaderProgramSource source = parseShader(filepath);
	m_ID = createShader(source.vertexSource, source.fragmentSource);
}

OpenGLShader::~OpenGLShader()
{
	glDeleteProgram(m_ID);
}

void OpenGLShader::bind() const
{
	glUseProgram(m_ID);
}

void OpenGLShader::unbind() const
{
	glUseProgram(0);
}

void OpenGLShader::setUniform1(const char* varname, const bool input)
{
	GLint location = glGetUniformLocation(m_ID, varname);
	SGSASSERT(location != -1, "Invalid uniform location");
	glUniform1i(location, input);
	//SGSASSERT(glGetError() == GL_NO_ERROR);
}

void OpenGLShader::setUniform1(const char* varname, const int input)
{
	GLint location = glGetUniformLocation(m_ID, varname);
	SGSASSERT(location != -1, "Invalid uniform location");
	glUniform1i(location, input);
	//SGSASSERT(glGetError() == GL_NO_ERROR);
}

void OpenGLShader::setUniform1(const char* varname, const float input)
{
	GLint location = glGetUniformLocation(m_ID, varname);
	SGSASSERT(location != -1, "Invalid uniform location");
	glUniform1f(location, input);
	//SGSASSERT(glGetError() == GL_NO_ERROR);
}

ShaderProgramSource OpenGLShader::parseShader(const std::string& filepath)
{
	std::ifstream stream(filepath);
	if(stream.is_open() == false)
	{
		SGSERROR("Shader file not opened!");
	}

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
	}
	//TODO: output stringstreams in tracer
	/*
	SGSTRACE("VERTEX SHADER: \n" + ss[0]);
	std::cout << "VERTEX SHADER: \n" + ss[0].str() << std::endl;
	std::cout << "FRAGMENT SHADER: \n" + ss[1].str() << std::endl;
	*/
	return { ss[0].str(), ss[1].str() };
}

u32 OpenGLShader::compileShader(u32 type, const std::string& source)
{
	u32 id = glCreateShader(type);
	// TODO: Solve the char to i8 issue
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	i32 isCompiled;
	glGetShaderiv(id, GL_COMPILE_STATUS, &isCompiled);
	if(isCompiled == GL_FALSE)
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

u32 OpenGLShader::createShader(const std::string& vertexSource, const std::string& fragmentSource)
{
	u32 program = glCreateProgram();
	u32 vertexShader = compileShader(GL_VERTEX_SHADER, vertexSource);
	u32 fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentSource);

	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);

	glLinkProgram(program);

	GLint isLinked = 0;
	glGetProgramiv(program, GL_LINK_STATUS, (i32*)&isLinked);
	if(isLinked == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

		std::vector<GLchar> infoLog(maxLength);
		glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

		glDeleteProgram(program);

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		SGSERROR("{0}", infoLog.data());
		SGSASSERT_MSG(false, "Shader link error!");

		return 0;
	}

	glDetachShader(program, vertexShader);
	glDetachShader(program, fragmentShader);

	return program;
}

u32 OpenGLShader::getUniformLocation(const std::string& name)
{
	if (m_uniformLocationCache.find(name) != m_uniformLocationCache.end())
		return m_uniformLocationCache[name];

	i32 location = glGetUniformLocation(m_ID, name.c_str());
	if (location == -1) {
		SGSWARN("Uniform %s doesn't exist", name);
	}
		//std::cout << "Warning: uniform '" << name << "' doesn't exist!" << std::endl;
	
	m_uniformLocationCache[name] = location;
	return location;
}
