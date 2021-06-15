#pragma once

#include "renderer/framebuffer.h"

class OpenGLFramebuffer : public Framebuffer
{
public:
	OpenGLFramebuffer(const FramebufferSpecs& specs);
	virtual ~OpenGLFramebuffer();

	void invalidate();

	virtual void bind() override;
	virtual void unbind() override;

	virtual const FramebufferSpecs& getSpecification() const override { return m_specs; }
	virtual u32 getColorAttachment() const override { return m_colorAttachment; }
	virtual void resize(u32 width, u32 height);
private:
	u32 m_ID = 0;
	u32 m_colorAttachment = 0, m_depthAttachment = 0;
	FramebufferSpecs m_specs;
};