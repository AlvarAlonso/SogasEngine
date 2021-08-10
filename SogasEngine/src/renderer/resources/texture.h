#pragma once

#include "core/assertions.h"
#include <memory>
#include <string>
#include <unordered_map>

namespace Sogas 
{
	// Exported to avoid warning C4275 when exporting Texture2D class
	class SGS Texture
	{
	public:
		virtual ~Texture() = default;

		virtual u32 getWidth() const = 0;
		virtual u32 getHeight() const = 0;

		virtual void setData(void* data, u32 size) = 0;
		virtual void setName(const std::string& name) = 0;
		virtual u32 getID() const = 0;
		virtual std::string& getName() = 0;

		virtual void bind(u32 slot = 0) const = 0;

	protected:
		std::string m_filename;
	};

	class SGS Texture2D : public Texture
	{
	public:
		static std::unordered_map<std::string, std::shared_ptr<Texture2D>> s_loadedTextures;

		static bool loadToMap(std::shared_ptr<Texture2D> texture, const std::string& name);
		static std::shared_ptr<Texture2D> create(u32 width, u32 height, void* data = nullptr);
		static std::shared_ptr<Texture2D> create(const std::string& path);
		static std::shared_ptr<Texture2D> GET(const std::string& filepath);
		static std::shared_ptr<Texture2D> GET(const u32 width, const u32 height, const std::string& filepath);

		virtual std::string& getName() = 0;
		virtual void setName(const std::string& name) = 0;
	};

	class SGS TextureCubeMap : public Texture
	{
	public:
		static std::shared_ptr<TextureCubeMap> GET(const std::string& filepath);
	private:
		std::shared_ptr<TextureCubeMap> create(const std::string& filepath);
	};
}
