#pragma once

#include "defines.h"

class VertexBuffer
{
private:
	u32 m_ID;
public:
	VertexBuffer(const void* data, u32 size);
	~VertexBuffer();

	void bind() const;
	void unbind() const;
};

