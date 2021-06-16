#pragma once

#include "renderer/resources/texture.h"

#include "../external/GLEW/glew-2.1.0/include/GL/glew.h"

class OpenGLTexture2D : public Texture2D
{
public:
	OpenGLTexture2D(u32 width, u32 height);
	OpenGLTexture2D(const std::string& path);
	virtual ~OpenGLTexture2D();

	virtual u32 getWidth() const override { return m_width; }
	virtual u32 getHeight() const override { return m_height; }

	virtual void setData(void* data, u32 size) override;
	virtual u32 getID() const override { return m_ID; };

	virtual void bind(u32 slot = 0) const override;
private:
	std::string m_path;
	u32 m_width;
	u32 m_height;
	u32 m_ID;
	GLenum m_internalFormat;
	GLenum m_dataFormat;
};