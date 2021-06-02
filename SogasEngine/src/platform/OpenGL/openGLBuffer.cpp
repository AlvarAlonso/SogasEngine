
#include "GL/glew.h"
#include "renderer/buffer.h"
#include "openGLBuffer.h"

OpenGLVertexBuffer::OpenGLVertexBuffer(f32* vertices, u32 size)
	: m_size(size)
{
	glCreateBuffers(1, &m_ID);
	glBindBuffer(GL_ARRAY_BUFFER, m_ID);
	glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
}

OpenGLVertexBuffer::~OpenGLVertexBuffer()
{
	glDeleteBuffers(1, &m_ID);
}

void OpenGLVertexBuffer::bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, m_ID);
}

void OpenGLVertexBuffer::unbind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

// -----------------------------------------------------------------------------------------------

OpenGLIndexBuffer::OpenGLIndexBuffer(u32* indices, u32 count)
	: m_count(count)
{
	glCreateBuffers(1, &m_ID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(u32), indices, GL_STATIC_DRAW);
}

OpenGLIndexBuffer::~OpenGLIndexBuffer()
{
	glDeleteBuffers(1, &m_ID);
}

void OpenGLIndexBuffer::bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, m_ID);
}

void OpenGLIndexBuffer::unbind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
