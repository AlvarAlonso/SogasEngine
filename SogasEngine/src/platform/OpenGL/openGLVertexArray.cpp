#include "renderer/vertexArray.h"

#include "GL/glew.h"
#include "openGLVertexArray.h"

#include "core/assertions.h"

static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
{
	switch(type)
	{
		case ShaderDataType::Float:    return GL_FLOAT;
		case ShaderDataType::Float2:   return GL_FLOAT;
		case ShaderDataType::Float3:   return GL_FLOAT;
		case ShaderDataType::Float4:   return GL_FLOAT;
		case ShaderDataType::Mat3:     return GL_FLOAT;
		case ShaderDataType::Mat4:     return GL_FLOAT;
		case ShaderDataType::Int:      return GL_INT;
		case ShaderDataType::Int2:     return GL_INT;
		case ShaderDataType::Int3:     return GL_INT;
		case ShaderDataType::Int4:     return GL_INT;
		case ShaderDataType::Bool:     return GL_BOOL;
	}

	SGSASSERT_MSG(false, "Unknown shader data type!");
	return 0;
}

OpenGLVertexArray::OpenGLVertexArray()
{
	glCreateVertexArrays(1, &m_ID);
}

OpenGLVertexArray::~OpenGLVertexArray()
{
	glDeleteVertexArrays(1, &m_ID);
}

void OpenGLVertexArray::bind() const
{
	glBindVertexArray(m_ID);
}

void OpenGLVertexArray::unbind() const
{
	glBindVertexArray(0);
}

void OpenGLVertexArray::addVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer)
{
	SGSASSERT_MSG(vertexBuffer->getLayout().getVertexAttributes().size(), "Vertex buffer has not layout!");

	glBindVertexArray(m_ID);
	vertexBuffer->bind();

	u32 index = 0;
	const auto& layout = vertexBuffer->getLayout();
	for(const auto& vertexAttribute : layout)
	{
		glEnableVertexAttribArray(index);
		glVertexAttribPointer(index,
			vertexAttribute.getComponentCount(),
			ShaderDataTypeToOpenGLBaseType(vertexAttribute.type),
			vertexAttribute.normalized ? GL_TRUE : GL_FALSE,
			layout.getStride(),
			(const void*)vertexAttribute.offset);
		index++;
	}

	m_vertexBuffers.push_back(vertexBuffer);
}

void OpenGLVertexArray::setIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer)
{
	glBindVertexArray(m_ID);
	indexBuffer->bind();

	m_indexBuffer = indexBuffer;
}
