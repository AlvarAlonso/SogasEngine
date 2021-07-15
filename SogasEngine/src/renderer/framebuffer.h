#pragma once

#include "defines.h"

namespace Sogas 
{
	enum class FramebufferTextureFormat
	{
		None = 0,

		// Color
		R8G8B8A8_FLOAT,
		R32_INT,

		// Depth/stencil
		D32_FLOAT,
		D24S8_FLOAT
	};

	struct FramebufferTextureSpecs
	{
		FramebufferTextureSpecs() = default;
		FramebufferTextureSpecs(FramebufferTextureFormat format)
			: textureFormat(format){}

		FramebufferTextureFormat textureFormat = FramebufferTextureFormat::None;
		// TODO: More properties to add
	};

	struct FramebufferAttachmentSpecs
	{
		FramebufferAttachmentSpecs() = default;
		FramebufferAttachmentSpecs(std::initializer_list<FramebufferTextureSpecs> initialAttachments)
			: attachments(initialAttachments) {}

		std::vector<FramebufferTextureSpecs> attachments;
	};

	struct FramebufferSpecs
	{
		u32 width{ 0 }, height{ 0 };
		FramebufferAttachmentSpecs attachments;
		u32 samples = 1;

		bool swapChainTarget = false;
	};

	class SGS Framebuffer
	{
	public:
		virtual ~Framebuffer() = default;
		virtual void bind() = 0;
		virtual void unbind() = 0;

		virtual void resize(u32 width, u32 height) = 0;
		virtual i32 readPixel(u32 attachmentIndex, i32 x, i32 y) = 0;

		virtual void clearAttachment(u32 attachmentIndex, i32 value) = 0;

		virtual u32 getColorAttachment(i32 index = 0) const = 0;
		
		virtual const FramebufferSpecs& getSpecification() const = 0;

		static Framebuffer* create(const FramebufferSpecs& specs);
	};
}