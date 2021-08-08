#include "sgspch.h"

#include "texture.h"

#include "renderer/renderer.h"
#include "platform/OpenGL/openGLTexture.h"

namespace Sogas 
{
	std::unordered_map<std::string, std::shared_ptr<Texture2D>> Texture2D::s_loadedTextures;

	bool Texture2D::loadToMap(std::shared_ptr<Texture2D> texture, const std::string& name)
	{
		if (!s_loadedTextures[name])
		{
			s_loadedTextures[name] = texture;
			return true;
		}

		return false;
	}

	std::shared_ptr<Texture2D> Texture2D::create(u32 width, u32 height, void* data)
	{
		switch (Renderer::getAPI())
		{
		case Renderer::API::None: SGSASSERT_MSG(false, "No graphics API selected");
		case Renderer::API::OpenGL: return std::make_shared<OpenGLTexture2D>(width, height, data);
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

	std::shared_ptr<Texture2D> Texture2D::GET(const std::string& filepath)
	{
		if (!s_loadedTextures[filepath])
		{
			std::shared_ptr<Texture2D> texture = Texture2D::create(filepath);
			texture->m_filepath = filepath;
			s_loadedTextures[filepath] = texture;
			return texture;
		}
		return s_loadedTextures[filepath];
	}

	std::shared_ptr<Texture2D> Texture2D::GET(const u32 width, const u32 height, const std::string& name)
	{
		if (!s_loadedTextures[name])
		{
			std::shared_ptr<Texture2D> texture = Texture2D::create(width, height);
			texture->m_filepath = name;
			texture->setName(name);
			s_loadedTextures[name] = texture;
			return texture;
		}
		return s_loadedTextures[name];
	}
}