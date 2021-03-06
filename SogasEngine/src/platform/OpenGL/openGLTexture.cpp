#include "sgspch.h"

#include "core/logger.h"
#include "openGLTexture.h"
#include "stb_image/stb_image.h"

namespace Sogas 
{
	OpenGLTexture2D::OpenGLTexture2D(u32 width, u32 height, void* data)
		: m_width(width), m_height(height)
	{
		m_internalFormat = GL_RGBA8;
		m_dataFormat = GL_RGBA;

		glCreateTextures(GL_TEXTURE_2D, 1, &m_ID);
		glTextureStorage2D(m_ID, 1, m_internalFormat, m_width, m_height);

		glTextureParameteri(m_ID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTextureParameteri(m_ID, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		glTextureParameteri(m_ID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_ID, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTextureSubImage2D(m_ID, 0, 0, 0, m_width, m_height, m_dataFormat, GL_UNSIGNED_BYTE, data);

		setData(data, height * width * 4);
	}

	OpenGLTexture2D::OpenGLTexture2D(const std::string& name)
	{
		i32 width, height, channels;
		stbi_set_flip_vertically_on_load(1);
		stbi_uc* data = stbi_load(name.c_str(), &width, &height, &channels, 0);

		SGSASSERT_MSG(data, "Failed to load image!");

		m_width = width;
		m_height = height;

		GLenum internalFormat = 0, dataFormat = 0;

		if (channels == 4)
		{
			internalFormat = GL_RGBA8;
			dataFormat = GL_RGBA;
		}
		else if (channels == 3)
		{
			internalFormat = GL_RGB8;
			dataFormat = GL_RGB;
		}

		m_internalFormat = internalFormat;
		m_dataFormat = dataFormat;

		SGSASSERT_MSG(internalFormat & dataFormat, "Format not supported!");

		glCreateTextures(GL_TEXTURE_2D, 1, &m_ID);
		glTextureStorage2D(m_ID, 1, internalFormat, m_width, m_height);

		glTextureParameteri(m_ID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_ID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureParameteri(m_ID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_ID, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTextureSubImage2D(m_ID, 0, 0, 0, m_width, m_height, dataFormat, GL_UNSIGNED_BYTE, data);

		stbi_image_free(data);
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		glDeleteTextures(1, &m_ID);
	}

	void OpenGLTexture2D::setData(void* data, u32 size)
	{
		u32 pixelBytes = m_dataFormat == GL_RGBA ? 4 : 3;
		SGSASSERT_MSG(size == m_width * m_height * pixelBytes, "Data must fill the entire texture!");
		glTextureSubImage2D(m_ID, 0, 0, 0, m_width, m_height, m_dataFormat, GL_UNSIGNED_BYTE, data);
	}

	void OpenGLTexture2D::bind(u32 slot) const
	{
		glBindTextureUnit(slot, m_ID);
		SGSASSERT(glGetError() == GL_NO_ERROR);
	}

	OpenGLTextureCubeMap::OpenGLTextureCubeMap(const std::string& filepath)
	{
		// TODO images are hardcoded right now
		std::vector<std::string> faces{
			"../Assets/textures/skybox/right.jpg",
			"../Assets/textures/skybox/left.jpg",
			"../Assets/textures/skybox/bottom.jpg",
			"../Assets/textures/skybox/top.jpg",
			"../Assets/textures/skybox/front.jpg",
			"../Assets/textures/skybox/back.jpg"
		};

		glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &m_ID);

		i32 width, height, channels;
		stbi_set_flip_vertically_on_load(true);
		for (u32 i = 0; i < faces.size(); ++i)
		{
			stbi_uc* data = stbi_load(faces[i].c_str(), &width, &height, &channels, 0);
			if (data) {
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
				stbi_image_free(data);
			}
			else {
				SGSWARN("Cubemap texture failed to load.");
				stbi_image_free(data);
			}
		}

		//stbi_set_flip_vertically_on_load(false);

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	}

	void OpenGLTextureCubeMap::bind(u32 slot) const
	{
		glBindTextureUnit(slot, m_ID);
		SGSASSERT(glGetError() == GL_NO_ERROR);
	}
}