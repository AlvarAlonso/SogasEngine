#include "sgspch.h"
#include "texture.h"

#include "renderer/renderer.h"
#include "core/utils.h"
#include "platform/OpenGL/openGLTexture.h"

namespace Sogas 
{
	std::unordered_map<std::string, std::shared_ptr<Texture2D>> Texture2D::s_loadedTextures;

	bool Texture2D::m_isInitialized = false;
	std::shared_ptr<TextureCubeMap> TextureCubeMap::s_cubeMapTexture;
	extern std::vector<std::string> g_assetsPath;

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

	bool Texture2D::initTextureResources()
	{
		if (m_isInitialized)
			return false;

		// default white texture
		u32 texData = 0xffffffff;
		std::shared_ptr<Texture2D> whiteTexture = create(1, 1, &texData);
		Texture2D::loadToMap(whiteTexture, "Default white");

		return true;
	}

	std::shared_ptr<Texture2D> Texture2D::GET(const std::string& filename)
	{
		if (!s_loadedTextures[filename])
		{
			std::string path = findFile(filename, g_assetsPath);
			std::shared_ptr<Texture2D> texture = Texture2D::create(path);
			texture->m_filename = filename;
			s_loadedTextures[filename] = texture;
			return texture;
		}
		return s_loadedTextures[filename];
	}

	std::shared_ptr<Texture2D> Texture2D::GET(const u32 width, const u32 height, const std::string& name)
	{
		if (!s_loadedTextures[name])
		{
			std::shared_ptr<Texture2D> texture = Texture2D::create(width, height);
			texture->m_filename = name;
			texture->setName(name);
			s_loadedTextures[name] = texture;
			return texture;
		}
		return s_loadedTextures[name];
	}

	std::shared_ptr<TextureCubeMap> TextureCubeMap::GET(const std::string& filename)
	{
		// TODO create a map for cube map textures
		//std::string filepath = findFile(filename, assetsPath);
		
		if(!s_cubeMapTexture)
			s_cubeMapTexture = TextureCubeMap::create(filename);

		return s_cubeMapTexture;
	}

	std::shared_ptr<TextureCubeMap> TextureCubeMap::create(const std::string& filepath)
	{
		switch (Renderer::getAPI())
		{
		case Renderer::API::None: SGSASSERT_MSG(false, "No graphics API selected");
		case Renderer::API::OpenGL: return std::make_shared<OpenGLTextureCubeMap>(filepath);
		}
	}
}