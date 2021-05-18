#include "VertexArray.h"

#include "../external/GLEW/glew-2.1.0/include/GL/glew.h"

VertexArray::VertexArray()
{
	glGenVertexArrays(1, &m_ID);
}

VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &m_ID);
}

void VertexArray::addBuffer(const VertexBuffer& vertexBuffer, const VertexBufferLayout& layout)
{
	bind();
	vertexBuffer.bind();
	
	const auto& elements = layout.getElements();
	u32 offset = 0;
	for(u32 i = 0; i < elements.size(); i++)
	{
		const auto& element = elements[i];
		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i, element.count, element.type, 
			element.normalized, layout.getStride(), (const void*)offset);
		offset += element.count * VertexBufferElement::getTypeSize(element.type);
	}
}

void VertexArray::bind() const
{
	glBindVertexArray(m_ID);
}

void VertexArray::unbind() const
{
	glBindVertexArray(0);
}
