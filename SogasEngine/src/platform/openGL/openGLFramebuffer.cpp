#include "sgspch.h"

#include "openGLFramebuffer.h"

#include "GLEW/glew-2.1.0/include/GL/glew.h"
#include "core/assertions.h"
#include "core/logger.h"

namespace Sogas 
{
	static const u32 s_maxFramebufferSize = 8192;

	static GLenum textureTarget(bool multisampled)
	{
		return multisampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
	}

	static void createTextures(bool multisampled, u32* outID, u32 count)
	{
		glCreateTextures(textureTarget(multisampled), count, outID);
	}

	static void bindTexture(bool multisampled, u32 id)
	{
		glBindTexture(textureTarget(multisampled), id);
	}

	static void attachColorTexture(u32 id, i32 samples, GLenum internalFormat, GLenum format, u32 width, u32 height, i32 index)
	{
		bool multisampled = samples > 1;
		if(multisampled)
		{
			glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, internalFormat, width, height, GL_FALSE);
		}
		else
		{
			glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, nullptr);
		
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		}

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, textureTarget(multisampled), id, 0);
	}

	static void attachDepthTexture(u32 id, i32 samples, GLenum format, GLenum attachmentType, u32 width, u32 height)
	{
		bool multisampled = samples > 1;
		if (multisampled)
		{
			glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, width, height, GL_FALSE);
		}
		else
		{
			glTexStorage2D(GL_TEXTURE_2D, 1, format, width, height);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		}

		glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, textureTarget(multisampled), id, 0);
	}

	static bool isDepthFormat(FramebufferTextureFormat format)
	{
		switch(format)
		{
			case FramebufferTextureFormat::D32_FLOAT: return true;
			case FramebufferTextureFormat::D24S8_FLOAT: return true;
		}

		return false;
	}

	static GLenum SogasFBTTextureFormatToGL(FramebufferTextureFormat format)
	{
		switch(format)
		{
			case FramebufferTextureFormat::R8G8B8A8_FLOAT: return GL_RGBA8;
			case FramebufferTextureFormat::R32_INT: return GL_RED_INTEGER; // TODO: check this type is 32 bits channel
		}

		SGSASSERT(false);
		return 0;
	}

	OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferSpecs& specs)
		: m_specs(specs)
	{
		for(auto spec : m_specs.attachments.attachments)
		{
			if (!isDepthFormat(spec.textureFormat))
				m_colorAttachmentSpecs.emplace_back(spec);
			else
				m_depthAttachmentSpecs = spec;
		}

		invalidate();
	}

	OpenGLFramebuffer::~OpenGLFramebuffer()
	{
		glDeleteFramebuffers(1, &m_ID);
		glDeleteTextures(m_colorAttachments.size(), m_colorAttachments.data());
		glDeleteTextures(1, &m_depthAttachment);
	}

	void OpenGLFramebuffer::invalidate()
	{
		if (m_ID)
		{
			glDeleteFramebuffers(1, &m_ID);
			glDeleteTextures(m_colorAttachments.size(), m_colorAttachments.data());
			glDeleteTextures(1, &m_depthAttachment);
		}

		glCreateFramebuffers(1, &m_ID);
		glBindFramebuffer(GL_FRAMEBUFFER, m_ID);

		bool multisample = m_specs.samples > 1;

		// attachments
		if(m_colorAttachmentSpecs.size())
		{
			m_colorAttachments.resize(m_colorAttachmentSpecs.size());
			createTextures(multisample, m_colorAttachments.data(), m_colorAttachments.size());

			for(size_t i = 0; i < m_colorAttachments.size(); i++)
			{
				bindTexture(multisample, m_colorAttachments[i]);
				switch(m_colorAttachmentSpecs[i].textureFormat)
				{
					case FramebufferTextureFormat::R8G8B8A8_FLOAT:
						attachColorTexture(m_colorAttachments[i], m_specs.samples, GL_RGBA8, GL_RGBA, m_specs.width, m_specs.height, i);
						break;
					case FramebufferTextureFormat::R32_INT:
						attachColorTexture(m_colorAttachments[i], m_specs.samples, GL_R32I, GL_RED_INTEGER, m_specs.width, m_specs.height, i);
						break;
				}
			}
		}

		if(m_depthAttachmentSpecs.textureFormat != FramebufferTextureFormat::None)
		{
			createTextures(multisample, &m_depthAttachment, 1);
			bindTexture(multisample, m_depthAttachment);
			switch(m_depthAttachmentSpecs.textureFormat)
			{
				case FramebufferTextureFormat::D24S8_FLOAT:
					attachDepthTexture(m_depthAttachment, m_specs.samples, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT, m_specs.width, m_specs.height);
					break;
				case FramebufferTextureFormat::D32_FLOAT:
					attachDepthTexture(m_depthAttachment, m_specs.samples, GL_DEPTH32F_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT, m_specs.width, m_specs.height);
					break;
			}
		}

		if(m_colorAttachments.size() > 1)
		{
			SGSASSERT(m_colorAttachments.size() <= 4);
			GLenum buffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
			glDrawBuffers(m_colorAttachments.size(), buffers);
		}
		else if(m_colorAttachments.empty())
		{
			glDrawBuffer(GL_NONE);
		}

		SGSASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete");

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFramebuffer::bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_ID);
		glViewport(0, 0, m_specs.width, m_specs.height);
	}

	void OpenGLFramebuffer::unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFramebuffer::resize(u32 width, u32 height)
	{
		if(width == 0 || height == 0 || width > s_maxFramebufferSize || height > s_maxFramebufferSize)
		{
			SGSWARN("Attempted to resize framebuffer to {0}, {1}", width, height);
		}

		m_specs.width = width;
		m_specs.height = height;

		invalidate();
	}

	i32 OpenGLFramebuffer::readPixel(u32 attachmentIndex, i32 x, i32 y)
	{
		SGSASSERT(attachmentIndex < m_colorAttachments.size());

		glReadBuffer(GL_COLOR_ATTACHMENT0 + attachmentIndex);
		i32 pixelData;
		glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixelData);
		
		return pixelData;
	}

	void OpenGLFramebuffer::clearAttachment(u32 attachmentIndex, i32 value)
	{
		SGSASSERT(attachmentIndex < m_colorAttachments.size());

		auto& spec = m_colorAttachmentSpecs[attachmentIndex];
		glClearTexImage(m_colorAttachments[attachmentIndex], 0,
			SogasFBTTextureFormatToGL(spec.textureFormat), GL_INT, &value);
	}
}