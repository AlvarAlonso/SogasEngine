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
private:
	u32 m_ID;
	u32 m_colorAttachment, m_depthAttachment;
	FramebufferSpecs m_specs;
};