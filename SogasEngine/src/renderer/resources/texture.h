#pragma once

#include "core/assertions.h"
#include <memory>
#include <string>

namespace Sogas 
{
	class Texture
	{
	public:
		virtual ~Texture() = default;

		virtual u32 getWidth() const = 0;
		virtual u32 getHeight() const = 0;

		virtual void setData(void* data, u32 size) = 0;
		virtual u32 getID() const = 0;

		virtual void bind(u32 slot = 0) const = 0;

	};

	class SGS Texture2D : public Texture
	{
	public:
		static std::shared_ptr<Texture2D> create(u32 width, u32 height);
		static std::shared_ptr<Texture2D> create(const std::string& path);
	};
}
