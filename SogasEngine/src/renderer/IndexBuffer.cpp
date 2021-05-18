#include "IndexBuffer.h"
#include "../external/GLEW/glew-2.1.0/include/GL/glew.h"

IndexBuffer::IndexBuffer(const void* data, u32 count)
	: m_count(count)
{
	glGenBuffers(1, &m_ID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(u32), data, GL_STATIC_DRAW);
}

IndexBuffer::~IndexBuffer()
{
	glDeleteBuffers(1, &m_ID);
}

void IndexBuffer::bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, m_ID);
}

void IndexBuffer::unbind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

u32 IndexBuffer::getCount() const
{
	return m_count;
}
