#include "sgspch.h"

#include "shader.h"

#include "renderer.h"
#include "platform/OpenGL/openGLShader.h"

namespace Sogas 
{
	std::unordered_map<std::string, std::shared_ptr<Shader>> Shader::s_loadedShaders;

	std::shared_ptr<Shader> Shader::create(const std::string& vertexSource, const std::string& fragmentSource)
	{
		switch (Renderer::getAPI())
		{
		case Renderer::API::None:  SGSASSERT_MSG(false, "No graphics API selected");
		case Renderer::API::OpenGL: return std::make_shared<OpenGLShader>(vertexSource, fragmentSource);
		}

		SGSASSERT_MSG(false, "Unknown renderer API!");
		return nullptr;
	}

	std::shared_ptr<Shader> Shader::create(const std::string& filepath)
	{
		switch (Renderer::getAPI())
		{
		case Renderer::API::None:  SGSASSERT_MSG(false, "No graphics API selected");
		case Renderer::API::OpenGL: return std::make_shared<OpenGLShader>(filepath);
		}

		SGSASSERT_MSG(false, "Unknown renderer API!");
		return nullptr;
	}

	std::shared_ptr<Shader> Shader::GET(const std::string filepath)
	{
		if (!Shader::s_loadedShaders[filepath])
		{
			std::shared_ptr<Shader> shader = Shader::create(filepath);
			s_loadedShaders[filepath] = shader;
			return shader;
		}
		return Shader::s_loadedShaders[filepath];
	}
}