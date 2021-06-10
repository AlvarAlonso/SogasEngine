#include "texture.h"

#include "renderer.h"
#include "platform/OpenGL/openGLTexture.h"

std::shared_ptr<Texture2D> Texture2D::create(u32 width, u32 height)
{
	switch(Renderer::getAPI())
	{
		case Renderer::API::None: SGSASSERT_MSG(false, "No graphics API selected");
		case Renderer::API::OpenGL: return std::make_shared<OpenGLTexture2D>(width, height);
	}

	SGSASSERT_MSG(false, "Unknown renderer API!");
	return nullptr;
}

std::shared_ptr<Texture2D> Texture2D::create(const std::string& path)
{
	switch (Renderer::getAPI())
	{
		case Renderer::API::None: SGSASSERT_MSG(false, "No graphics API selected");
		case Renderer::API::OpenGL: return std::make_shared<OpenGLTexture2D>(path);
	}

	SGSASSERT_MSG(false, "Unknown renderer API!");
	return nullptr;
}