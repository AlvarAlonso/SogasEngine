#pragma once

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

class VertexArray
{
private:
	u32 m_ID;
public:
	VertexArray();
	~VertexArray();

	void addBuffer(const VertexBuffer& vertexBuffer, const VertexBufferLayout& layout);

	void bind() const;
	void unbind() const;
};

