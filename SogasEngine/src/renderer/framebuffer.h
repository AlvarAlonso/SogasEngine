#pragma once

#include "defines.h"

namespace Sogas 
{
	struct FramebufferSpecs
	{
		u32 width{ 0 }, height{ 0 };
		// Format?
		bool swapChainTarget = false;
	};

	class SGS Framebuffer
	{
	public:
		virtual ~Framebuffer() = default;
		virtual void bind() = 0;
		virtual void unbind() = 0;

		virtual u32 getColorAttachment() const = 0;
		virtual const FramebufferSpecs& getSpecification() const = 0;
		virtual void resize(u32 width, u32 height) = 0;

		static Framebuffer* create(const FramebufferSpecs& specs);

	};
}