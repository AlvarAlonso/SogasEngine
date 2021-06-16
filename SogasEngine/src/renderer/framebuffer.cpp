#include "framebuffer.h"

#include "renderer/renderer.h"
#include "platform/openGL/openGLFramebuffer.h"

namespace Sogas 
{
	Framebuffer* Framebuffer::create(const FramebufferSpecs& specs)
	{
		switch ((u16)Renderer::getAPI())
		{
		case (u16)RendererAPI::API::None: SGSASSERT(false); return nullptr;
		case (u16)RendererAPI::API::OpenGL: return new OpenGLFramebuffer(specs);
		}

		SGSASSERT(false);
		return nullptr;
	}
}