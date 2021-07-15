#pragma once

#include "renderer/framebuffer.h"

namespace Sogas 
{
	class OpenGLFramebuffer : public Framebuffer
	{
	public:
		OpenGLFramebuffer(const FramebufferSpecs& specs);
		virtual ~OpenGLFramebuffer();

		void invalidate();

		virtual void bind() override;
		virtual void unbind() override;

		virtual void resize(u32 width, u32 height) override;
		virtual i32 readPixel(u32 attachmentIndex, i32 x, i32 y) override;

		virtual void clearAttachment(u32 attachmentIndex, i32 value) override;

		virtual u32 getColorAttachment(i32 index = 0) const override { return m_colorAttachments[index]; }

		virtual const FramebufferSpecs& getSpecification() const override { return m_specs; }
	private:
		u32 m_ID = 0;
		FramebufferSpecs m_specs;

		std::vector<FramebufferTextureSpecs> m_colorAttachmentSpecs;
		FramebufferTextureSpecs m_depthAttachmentSpecs = FramebufferTextureFormat::None;

		std::vector<u32> m_colorAttachments;
		u32 m_depthAttachment = 0;
	};
}