#include "sgspch.h"

#include "shader.h"

#include "renderer.h"
#include "platform/OpenGL/openGLShader.h"

namespace Sogas 
{
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
}