#include "sgspch.h"

#include "GL/glew.h"
#include "renderer/buffer.h"
#include "openGLBuffer.h"

namespace Sogas 
{
	OpenGLVertexBuffer::OpenGLVertexBuffer(f32* vertices, u32 size)
		: m_size(size)
	{
		glCreateBuffers(1, &m_ID);
		glBindBuffer(GL_ARRAY_BUFFER, m_ID);
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
		SGSASSERT(glGetError() == GL_NO_ERROR);
	}

	OpenGLVertexBuffer::OpenGLVertexBuffer(const void* data, u32 size)
		: m_size(size)
	{
		glCreateBuffers(1, &m_ID);
		glBindBuffer(GL_ARRAY_BUFFER, m_ID);
		glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
		SGSASSERT(glGetError() == GL_NO_ERROR);
	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{
		glDeleteBuffers(1, &m_ID);
		SGSASSERT(glGetError() == GL_NO_ERROR);
	}

	void OpenGLVertexBuffer::bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_ID);
		SGSASSERT(glGetError() == GL_NO_ERROR);
	}

	void OpenGLVertexBuffer::unbind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		SGSASSERT(glGetError() == GL_NO_ERROR);
	}

	// -----------------------------------------------------------------------------------------------

	OpenGLIndexBuffer::OpenGLIndexBuffer(u32* indices, u32 count)
		: m_count(count)
	{
		glCreateBuffers(1, &m_ID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(u32), indices, GL_STATIC_DRAW);
		SGSASSERT(glGetError() == GL_NO_ERROR);
	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		glDeleteBuffers(1, &m_ID);
		SGSASSERT(glGetError() == GL_NO_ERROR);
	}

	void OpenGLIndexBuffer::bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_ID);
		SGSASSERT(glGetError() == GL_NO_ERROR);
	}

	void OpenGLIndexBuffer::unbind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		SGSASSERT(glGetError() == GL_NO_ERROR);
	}
}